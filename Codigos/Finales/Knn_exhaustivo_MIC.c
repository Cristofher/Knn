#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <values.h>
#include <omp.h>
#include <math.h>
//para el calculo de tiempos
#include <sys/resource.h>
#include <time.h>
#include <sys/time.h>

#define ERROR -1


FILE *Salida;

struct _Elem
{
    double dist;
    int ind;
};
typedef struct _Elem Elem;

int leedato(double *dato,FILE *file, int DIM,int dimaux);
void copiavalor(double *a, double *b, int DIM,int dimaux);
void scanFile(char *ruta, double **datos, int DIM, int numElement,int dimaux);
void matrixToVector(double **matrix, int num_cols, int num_rows, double *vector);
// el "__attribute__((target(mic)))" es para que la función sea compilada para ejecutarsse en la MIC
__attribute__((target(mic))) void inserta2(Elem *heap, Elem *elem, int *n_elem);
__attribute__((target(mic))) void extrae2(Elem *heap, int *n_elem, Elem *elem_extraido);
__attribute__((target(mic))) void popush2(Elem *heap, int *n_elem, Elem *elem);
__attribute__((target(mic))) double topH(Elem *heap, int *n_elem);
__attribute__((target(mic))) double distancia(double *p1, double *p2, int DIM);


int main(int argc, char *argv[]){
   char *ruta_db, *ruta_queries;
	 double **db, **queries, *db_vector, *queries_vector;
   int num_db, num_queries, dim, k, i, j;
   int num_threads, thread_num;
   Elem *answer;
   //variables para medir el tiempo
   struct rusage r1, r2;
   double user_time, sys_time, real_time;
   struct timeval t1, t2;
  if (argc != 7){
		printf("Error :: Ejecutar como : main.out archivo_BD Num_elem archivo_queries Num_queries dim k\n");
		return 1;
	}
  
   ruta_db = (char *)malloc(sizeof(char)*(strlen(argv[1])+1));
   strcpy(ruta_db, argv[1]);
   num_db = atoi(argv[2]);

   ruta_queries = (char *)malloc(sizeof(char)*(strlen(argv[3])+1));
   strcpy(ruta_queries, argv[3]);
   num_queries = atoi(argv[4]);
   int dimaux=0,add=0;
   dim = atoi(argv[5]);
   k = atoi(argv[6]);

   int validamod=dim%16;
   if(validamod!=0){
   	if (dim<16){
   		dimaux=16;
   	}else{
   		add = 16-validamod;
   		dimaux=dim+add;
   	}
   }else{
   	dimaux=0;
   }

   fflush(stdout);

   db= (double **)malloc(sizeof(double *)*num_db);
   for (i=0; i<num_db; i++)
      db[i] = (double *)malloc(sizeof(double)*dimaux);
   queries = (double **)malloc(sizeof(double *)*num_queries);
   for (i=0; i<num_queries; i++)
      queries[i] = (double *)malloc(sizeof(double)*dimaux);
   answer = (Elem *)malloc(sizeof(Elem)*num_queries*k);
   //scan DB
   scanFile(ruta_db, db, dim, num_db,dimaux);
   scanFile(ruta_queries, queries, dim, num_queries,dimaux);

   //Se transfieren datos de una matriz a un vector
   db_vector = (double *)_mm_malloc(sizeof(double)*dimaux*num_db, 64);
   for (i=0; i < dimaux*num_db; i++)
       db_vector[i] = 0.0;
   if (sizeof(double)*dimaux*num_queries < 64)
   {
        queries_vector = (double *)_mm_malloc(sizeof(double)*16, 64);
        for (i=0; i < 16; i++)
            queries_vector[i] = 0.0;
   }
   else
   {
        queries_vector = (double *)_mm_malloc(sizeof(double)*dimaux*num_queries, 64);
        for (i=0; i < dimaux*num_queries; i++)
            queries_vector[i] = 0.0;
   }
   matrixToVector(db, dimaux, num_db, db_vector);
   matrixToVector(queries, dimaux, num_queries, queries_vector);
   fflush(stdout);
   //inicio de la medida de tiempo    
   getrusage(RUSAGE_SELF, &r1);
   gettimeofday(&t1, 0);

   
   #pragma offload target(mic:0) in(dim) in(db_vector:length(num_db*dimaux)) in(queries_vector:length(num_queries*dimaux)) out(answer:length(k*num_queries))
   {
      #pragma omp parallel private(i, j, thread_num) shared(db_vector, num_db, queries_vector, num_queries, dimaux, k, answer, num_threads)
      {
         
         Elem *heap;
         heap = (Elem *)malloc(sizeof(Elem)*k);
         #pragma omp master
         {
            num_threads = omp_get_num_threads();
            printf("run with %d threads\n", num_threads);
         }
         #pragma omp barrier
         thread_num = omp_get_thread_num();
         int n_elem;
         Elem e_temp;
         double d;

         
         for(i=thread_num*dimaux; i<num_queries*dimaux; i+=num_threads*dimaux){
            n_elem = 0;
            for(j=0; j<k; j++){
               e_temp.dist = distancia(&(queries_vector[i]), &(db_vector[j*dimaux]), dimaux);
               e_temp.ind = j;
               inserta2(heap, &e_temp, &n_elem);
            }

            for(j=k; j<num_db; j++){
               d = distancia(&(queries_vector[i]), &(db_vector[j*dimaux]), dimaux);
               if(d < topH(heap, &n_elem))
                  {
                     e_temp.dist = d;
                     e_temp.ind = j;
                     popush2(heap, &n_elem, &e_temp);
                  }
            }

            for(j=0; j<k; j++){
               extrae2(heap, &n_elem, &e_temp);
               printf("%d ind = %d :: dist = %f posición:: %d \n", j, e_temp.ind, e_temp.dist,(i/dimaux)*k+j);
               answer[(i/dimaux)*k+j].ind = e_temp.ind;
               answer[(i/dimaux)*k+j].dist = e_temp.dist;
            }


         }
         free(heap);
      }
   }   
   gettimeofday(&t2, 0);
   getrusage(RUSAGE_SELF, &r2);
   Salida = fopen("Salida.txt", "w");
    for (i = 0; i < num_queries; ++i){
      fprintf(Salida, "Consulta id:: %d\n",i);
      for (j = 0; j < k; ++j)
      {
        fprintf(Salida,"ind = %d :: dist = %f\n",answer[(i*k)+j].ind,answer[(i*k)+j].dist);
      }
      fprintf(Salida, "---------------------------------\n");
    }
    fclose(Salida);

   user_time = (r2.ru_utime.tv_sec - r1.ru_utime.tv_sec) + (r2.ru_utime.tv_usec - r1.ru_utime.tv_usec)/1000000.0;
   sys_time = (r2.ru_stime.tv_sec - r1.ru_stime.tv_sec) + (r2.ru_stime.tv_usec - r1.ru_stime.tv_usec)/1000000.0;
   real_time = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
   printf("\nCPU Time = %lf", sys_time + user_time);
   printf("\nReal Time = %lf\n", real_time);
   
   return 0;
}



void scanFile(char *ruta, double **datos, int DIM, int numElement, int dimaux)
{
    double *dato;
    int i;
    FILE *f_dist;

    dato = (double *)malloc(sizeof(double)*DIM);
    
    printf("\nAbriendo %s... ", ruta);
    
    f_dist = fopen(ruta, "r");
    printf("OK\n");
    fflush(stdout);
    
    printf("\nLeyendo DB... ");
    fflush(stdout);
    for (i=0; i<numElement; i++)
    {
        if (feof(f_dist) || leedato(dato, f_dist, DIM, dimaux)==ERROR)//Si se alcanzo el final del archivo o hay un error
        {
            printf("\n\nERROR :: N_DB mal establecido - 1\n\n");
            fflush(stdout);
            fclose(f_dist);
            break;
        }
        copiavalor(datos[i], dato, DIM, dimaux);
    }
    fclose(f_dist);//Cerrando el archivo que se abrio con fopen
    free(dato);
    printf("OK\n");
    
    fflush(stdout);
}


int leedato(double *dato,FILE *file, int DIM, int dimaux){

    int i=0, temp;
    double num_f;
    for (i=0;i<DIM;i++){
        if ((temp = fscanf(file, "%lf", &num_f)) < 1){
            printf("\nERROR :: leedato :: fscanf :: temp = %d\n", temp);
            fflush(stdout);
            return ERROR;
        }       
        dato[i] = (double)num_f;      
        if (i+1 < DIM)
            if (fgetc(file) != ','){
                printf("\nERROR :: ',' no encontrada\n");
                return ERROR;
            }
    }
    return 1;
}

void copiavalor(double *a, double *b, int DIM, int dimaux)
{
    int i;
    for (i=0; i<DIM; i++){
        a[i] = b[i];
    }
    for (i = DIM; i < dimaux; ++i){
    	a[i] = 0;
    }
    return;
}

__attribute__((target(mic))) void inserta2(Elem *heap, Elem *elem, int *n_elem)
{
    int i;
    Elem temp;
    
    heap[*n_elem].dist = elem->dist;
    heap[*n_elem].ind = elem->ind;
    (*n_elem)++;
    for (i = *n_elem; i>1 && heap[i-1].dist > heap[(i/2)-1].dist; i=i/2)
    {
        //Intercambiamos con el padre
        temp = heap[i-1];
        heap[i-1] = heap[(i/2)-1];
        heap[(i/2)-1] = temp;
    }
}

//inserta2 y extrae2 usan el arreglo del heap desde el elemento 0
__attribute__((target(mic))) void extrae2(Elem *heap, int *n_elem, Elem *elem_extraido)
{
    int i, k;
    Elem temp;
    
    (*elem_extraido).dist = heap[0].dist;
    (*elem_extraido).ind = heap[0].ind;
    
    heap[0] = heap[(*n_elem)-1];  // Movemos el ultimo a la raiz y achicamos el heap
    (*n_elem)--;
    i = 1;
    while(2*i <= *n_elem) // mientras tenga algun hijo
    {
        k = 2*i; //el hijo izquierdo
        if(k+1 <= *n_elem && heap[(k+1)-1].dist > heap[k-1].dist)
            k = k+1;  //el hijo derecho es el mayor
        if(heap[i-1].dist > heap[k-1].dist)
            break;  //es mayor que ambos hijos
        
        temp = heap[i-1];
        heap[i-1] = heap[k-1];
        heap[k-1] = temp;
        i = k;   //lo intercambiamos con el mayor hijo
    }
    return;
}

__attribute__((target(mic))) void popush2(Elem *heap, int *n_elem, Elem *elem)
{
    int i, k;
    Elem temp;

	//printf("\nelem->dist = %lf :: elem->ind = %d\n", (*elem).dist, elem->ind);
	//printf("\nn_elem = %d\n", *n_elem);
    
    heap[0].dist = elem->dist;
    heap[0].ind = elem->ind;
    
    i = 1;
    while(2*i <= (*n_elem)) // mientras tenga algun hijo
    {
        k = 2*i; //el hijo izquierdo
        if(k+1 <= (*n_elem) && heap[(k+1)-1].dist > heap[k-1].dist)
            k = k+1;  //el hijo derecho es el mayor
        if(heap[i-1].dist > heap[k-1].dist)
            break;  //es mayor que ambos hijos
        
        temp = heap[i-1];
        heap[i-1] = heap[k-1];
        heap[k-1] = temp;
        i = k;   //lo intercambiamos con el mayor hijo
    }
    return;
}

__attribute__((target(mic))) double topH(Elem *heap, int *n_elem)
{
    if ((*n_elem) == 0)
        return MAXDOUBLE;
    return heap[0].dist;
}

void matrixToVector(double **matrix, int num_cols, int num_rows, double *vector)
{
    int i,j;
    for(i=0; i<num_rows; i++)
        for(j=0; j<num_cols; j++)
            vector[(i*num_cols)+j] = matrix[i][j];
}

__attribute__((target(mic))) double distancia(double *p1, double *p2, int DIM)
{
    int i=0;
    double suma=0.0;
    double aux, aux2;
   __assume_aligned(p1, 64);
   __assume_aligned(p2, 64);   
    #pragma vector aligned
    #pragma ivdep
    #pragma simd
    for (i=0; i < DIM; i++){
        suma += (p1[i]-p2[i])*(p1[i]-p2[i]);
    }
    
    return sqrt(suma);
}
