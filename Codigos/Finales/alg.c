#include <stdio.h>
#include <cuda.h>
#include <float.h>
//para el calculo de tiempos
#include <sys/resource.h>
#include <time.h>
#include <sys/time.h>

//Libreria con valores
#include "libreria.h"

#define DIM DEFINE_DIMENSION
//#define DIM 254
//#define DIM 64

/* T_x_BLOCK es la cantidad de hilos por Bloque. Si la BD tiene menos elementos que T_x_BLOCK, se ejecutan tantos hilos como elementos hayan */
#define T_x_BLOCK devProp_maximumThreadsPerBlock 

#define ERROR -1

#define TOPK DEFINE_TOPK

//#define NE 95325 //Num de elementos
//#define NE 499865
//#define NE 999987
//#define NE 1699798
//#define NE 200000
//#define NE 500000
//#define NE 1000000
//#define NE 1500000
//#define NE 2000000
//#define NE 8480 //Num de elementos
#define NE DEFINE_N_ELEM

#define TAM_WARP 32 //Num de threads maximo de un warp


/* El valor Q es la cantidad de consultas lanzadas en un kernel. Q depende de la cantidad de memoria en la GPU */
//#define Q 3972 //NASA_80
//#define Q 2979 //NASA_200000
//#define Q 1324 //NASA_500000
//#define Q 662 //NASA_999996
#define Q 3999 //NASA_1500000 // Cophir 500000
//#define Q 331 //NASA_2000000 // Cophir 1000000 // Cophir 1700000

struct _Elem
{
  double dist;
  int ind;
};
typedef struct _Elem Elem;

__device__ void insertaH(Elem *heap, Elem *elem, int *n_elem, int pitch, int id);
__device__ void extraeH(Elem *heap, int *n_elem, int pitch, int id, Elem *eresult);
__device__ double topH(Elem *heap, int id);
__device__ void popush(Elem *heap, Elem *elem, int *n_elem, int pitch, int id);
__device__ double distancia_trans(double *p1, int pitch_p1, int col_1, double *q);
void imprime_trans(double **MAT, int col);
int leedato(double *dato, FILE *file);
int leedato_cophir(double *dato, FILE *file);
int leedato_trans(double **dato, FILE *file, int col);
int leedato_trans_cophir(double **dato, FILE *file, int col);
                                                                                                                                                                                               __global__ void Batch_Heap_Reduction(double *DB_dev, int pitch_DB, Elem *heap, int pitch_H, double *QUERY_dev, int pitch_QUERY, Elem *arr_Dist, int pitch_Dist, int beginQ, double *res_final)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      

int N_QUERIES;
//double vectores[DIM][NE];


main(int argc, char *argv[])
{
   int i, N_ELEM, dimension, j;
   FILE *pf;
   double **vectores;
   struct rusage r1, r2;
   double user_time, sys_time, real_time;
   struct timeval t1, t2;
   double *Elems, *QUERY_dev;
   double **consultas, *res_final, *res_final_H;
   int retorno, T_per_BLOCK, N_BLOQUES, contQ, cont;
   Elem *HEAPS_dev, *arr_Dist;
   size_t pitch, pitch_H, pitch_Q, pitch_Dist;  
   double prom, prom_cont;

//   cudaSetDevice(1);
  printf("#define DEFINE_TOPK     %d\n",  DEFINE_TOPK);
  printf("#define TOPK     %d\n",  TOPK);

   if (argc != 1)
   {
      printf("\nEjecutar como: a.out archivo_BD archivo_queries N_ELEM N_QUERIES DIM\n");
      return 0;
   }
   if ((pf = fopen(DEFINE_archivo_BD, "r")) == NULL)
   {
      printf("\nNo se pudo abrir el archivo %s\n" ,DEFINE_archivo_BD);
      return 0;
   }

   N_ELEM = DEFINE_N_ELEM;
   N_QUERIES = DEFINE_N_QUERIES;
   dimension = DEFINE_DIMENSION;

   if (dimension != DIM )
   {
     printf("\nERROR :: dimension != DIM\n");
     return 0;
   }

   printf("\nCant. Elementos=%d :: dimension=%d\n" , N_ELEM, dimension);
   fflush(stdout);
   if (N_ELEM != NE)
   {
     printf("\nERORR :: N_ELEM != NE\n");
     return 0;
   }

   if (TOPK > N_ELEM){
      printf("ERROR  :: TOPK muy grande debe ser menor de numero de elementos de la base de datos\n");
   }

   if (T_x_BLOCK > N_ELEM)
        T_per_BLOCK = N_ELEM;
   else
        T_per_BLOCK = T_x_BLOCK;

   if (cudaSuccess != cudaMalloc((void **)&res_final, sizeof(double)*Q*TOPK))
   {
     printf("\nERROR 21 :: cudaMalloc\n");
     cudaThreadExit();
     return 0;
   }
   res_final_H = (double *)malloc(sizeof(double)*Q*TOPK);
   for (i=0; i<Q*TOPK; i++)
   {
      res_final_H[i] = 0;
   }
   if (cudaSuccess != cudaMemset(res_final, 0, sizeof(double)*Q*TOPK))
   {
       printf("\nERROR :: cudaMemset\n");
       cudaThreadExit();
       return 0;
   }

   //HEAPS_dev[TOPK][Q*T_per_BLOCK]
   if (cudaSuccess != cudaMallocPitch((void **)&HEAPS_dev, &pitch_H, sizeof(Elem)*Q*T_per_BLOCK, (size_t)TOPK))
   {
      printf("\nERROR 4 :: cudaMallocPitch :: Heaps_dev col=%lld :: row=%d\n", (long long)(sizeof(Elem)*Q*T_per_BLOCK), TOPK);
      cudaThreadExit();
      return 0;
   }

   Elem *linea_temp = (Elem *)malloc(sizeof(Elem)*Q*T_per_BLOCK);
   for (i=0 ; i < Q*T_per_BLOCK; i++)
   {
       linea_temp[i].ind  = -1;
       linea_temp[i].dist = DBL_MAX; //DBL_MAX es el maximo valor para un double segun float.h
   }
   for (i=0 ; i < TOPK; i++)
        if (cudaSuccess != cudaMemcpy((Elem *)((char *)HEAPS_dev + (i*(int)pitch_H)), (Elem *)linea_temp, sizeof(Elem)*Q*T_per_BLOCK, cudaMemcpyHostToDevice))
        {
            printf("\nERROR :: cudaMemcpy\n");
            cudaThreadExit();
            return 0;
        }

   //arr_Dist[Q][N_ELEM]
   if (cudaSuccess != cudaMallocPitch((void **)&arr_Dist, &pitch_Dist, N_ELEM*sizeof(Elem), (size_t)Q))
   {
      printf("\nERROR 41 :: cudaMallocPitch\n");
      cudaThreadExit();
      return 0;
   }

   vectores =(double **)malloc(sizeof(double *)*dimension);
   for (i=0; i<dimension; i++)
      vectores[i] = (double *)malloc(sizeof(double)*N_ELEM);

   for (i=0; i<N_ELEM; i++)
   {
//      printf("Leyendo vectores[%d] : ", i);
      for (j=0; j<dimension; j++)
      {
         fscanf(pf, "%lf", &vectores[j][i]);
//         printf("%lf ", vectores[i][j]);
      }
//      printf("\n");
      fgetc(pf);
   }
   fclose(pf);

   //Elems[dimension][N_ELEM]
   if (cudaSuccess != cudaMallocPitch((void **)&Elems, (size_t *)&pitch, N_ELEM*sizeof(double), (size_t)dimension))
      printf("\nERROR :: cudaMallocPitch 4\n");

   for (i=0; i < dimension; i++)
   {
      retorno = cudaMemcpy((double *)((char *)Elems + (i*(int)pitch)), (double *)(vectores[i]), sizeof(double)*N_ELEM, cudaMemcpyHostToDevice);
     if (retorno != cudaSuccess)
     {
      switch(retorno)
      {
       case cudaErrorInvalidPitchValue:
         printf("\nERROR 2 -> cudaErrorInvalidPitchValue:\n");
         break;
       case cudaErrorInvalidDevicePointer:
         printf("\nERROR 2 -> cudaErrorInvalidDevicePointer:\n");
         break;
       case cudaErrorInvalidMemcpyDirection:
         printf("\nERROR 2 -> cudaErrorInvalidMemcpyDirection:\n");
         break;
       case cudaErrorInvalidValue:
         printf("\nERROR 2 -> cudaErrorInvalidValue :: i=%d :: pitch=%d\n", i, pitch);
         break;
       default: 
         printf("\nERROR 2 -> Checkear esto.\n");
         break;
      }
      return 0;
     }
   }

   consultas =(double **)malloc(sizeof(double *)*N_QUERIES);
   for (i=0; i<N_QUERIES; i++)
      consultas[i] = (double *)malloc(sizeof(double)*dimension);

   //Leo las queries
   if ((pf = fopen(DEFINE_archivo_queries, "r")) == NULL)
   {
      printf("\nNo se pudo abrir el archivo %s\n" ,DEFINE_archivo_queries);
      return 0;
   }
/*
   fgets(linea, tam_lin-1, pf);
   fscanf(pf, "%d", &N_QUERIES);
   fscanf(pf, "%d", &dimension);
   fscanf(pf, "%d", &tam_elem);
   fgetc(pf);
   */
   printf("\n\nArchivo de Queries:\nCant. Elementos=%d :: dimension=%d\n" , N_QUERIES, dimension);

   for (i=0; i<N_QUERIES; i++)
   {a
	if (leedato(consultas[i], pf) == -1)
	{
		printf("\nError al leer Consultas\n");
         	cudaThreadExit();
		return 0;
	}
   }
   fclose(pf);

   //QUERY_dev[N_QUERIES][dimension]
   if (cudaSuccess != cudaMallocPitch((void **)&QUERY_dev, (size_t *)&pitch_Q, dimension*sizeof(double), (size_t)N_QUERIES))
      printf("\nERROR :: cudaMallocPitch 1\n");

   for (i=0; i < N_QUERIES; i++)
   {
     if (cudaSuccess != cudaMemcpy((char *)QUERY_dev + (i*(int)pitch_Q), consultas[i], sizeof(double)*dimension, cudaMemcpyHostToDevice))
       printf("\nERROR 3 :: cudaMemcpy\n");
   }

   N_BLOQUES = Q;
   contQ = 0;
   cont = 0;
   getrusage(RUSAGE_SELF, &r1);
   gettimeofday(&t1, 0);

   while(contQ < N_QUERIES)
   {
      contQ += Q;
      if (contQ > N_QUERIES)
         N_BLOQUES = N_QUERIES - (contQ-Q);
      printf("\nN_BLOQUES = %d :: T_per_BLOCK = %d\n", N_BLOQUES, T_per_BLOCK);

      Batch_Heap_Reduction<<< N_BLOQUES, T_per_BLOCK>>> (Elems, (int)pitch, HEAPS_dev, (int)pitch_H, QUERY_dev, (int)pitch_Q, arr_Dist, (int)pitch_Dist, Q*cont, res_final);

      if (cudaSuccess != cudaMemcpy((double *)res_final_H, (double *)res_final, sizeof(double)*Q*TOPK, cudaMemcpyDeviceToHost))
      {
         printf("\nERROR 41 :: cudaMemcpy :: iteraH\n");
         cudaThreadExit();
         return 0;
      }
      cont++;
   }

   gettimeofday(&t2, 0);
   getrusage(RUSAGE_SELF, &r2);

    for (i=0; i<N_BLOQUES; i++)
    {
        printf("\n\nResults array %d (smallest distances):", i);  
        for (j=TOPK*i; j<(TOPK*i)+TOPK; j++)
            printf("\nquery = %d :: dist = %lf", i, res_final_H[j]);
    }
    printf("\n");

   user_time = (r2.ru_utime.tv_sec - r1.ru_utime.tv_sec) + (r2.ru_utime.tv_usec - r1.ru_utime.tv_usec)/1000000.0;
   sys_time = (r2.ru_stime.tv_sec - r1.ru_stime.tv_sec) + (r2.ru_stime.tv_usec - r1.ru_stime.tv_usec)/1000000.0;
   real_time = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;

      prom = 0;
      prom_cont = 0;
      for (i=0; i<Q; i++)
      {
         if (res_final_H[i] != 0)
         {
            prom += res_final_H[i];
            prom_cont += 1;
         }
      }

      printf("\nK = %d", TOPK);
	  	printf("\nTiempo CPU = %f", user_time + sys_time);
	  	printf("\nTiempo Real = %f", real_time);
      printf("\nprom = %lf\n", (double)(prom/(double)prom_cont));
      fflush(stdout);

   cudaFree(Elems);
   cudaFree(QUERY_dev);
   cudaFree(HEAPS_dev);
   cudaFree(arr_Dist);

  cudaThreadExit();
  return 0;
}


__device__ void insertaH(Elem *heap, Elem *elem, int *n_elem, int pitch, int id)
{
  int i;
  Elem temp;

  ((Elem *)((char *)heap + (*n_elem)*pitch))[id].dist = elem->dist;
  ((Elem *)((char *)heap + (*n_elem)*pitch))[id].ind = elem->ind;
  (*n_elem)++;
  for (i = *n_elem; i>1 && ((Elem *)((char *)heap + (i-1)*pitch))[id].dist > ((Elem *)((char *)heap + ((i/2)-1)*pitch))[id].dist; i=i/2)
  {
    //Intercambiamos con el padre
    temp.dist = ((Elem *)((char *)heap + (i-1)*pitch))[id].dist;
    temp.ind = ((Elem *)((char *)heap + (i-1)*pitch))[id].ind;
    ((Elem *)((char *)heap + (i-1)*pitch))[id].dist = ((Elem *)((char *)heap + ((i/2)-1)*pitch))[id].dist;
    ((Elem *)((char *)heap + (i-1)*pitch))[id].ind = ((Elem *)((char *)heap + ((i/2)-1)*pitch))[id].ind;
    ((Elem *)((char *)heap + ((i/2)-1)*pitch))[id].dist = temp.dist;
    ((Elem *)((char *)heap + ((i/2)-1)*pitch))[id].ind = temp.ind;
  }
  return;
}

__device__ void extraeH(Elem *heap, int *n_elem, int pitch, int id, Elem *eresult)
{
  int i, k;
  Elem temp;
  eresult->dist = ((Elem *)((char *)heap+0))[id].dist; //Se guarda el maximo
  eresult->ind = ((Elem *)((char *)heap+0))[id].ind; 

  ((Elem *)((char *)heap+0))[id].dist = ((Elem *)((char *)heap + ((*n_elem)-1)*pitch))[id].dist;// Movemos el ultimo a la raiz y achicamos el heap
  ((Elem *)((char *)heap+0))[id].ind = ((Elem *)((char *)heap + ((*n_elem)-1)*pitch))[id].ind;
  (*n_elem)--;
  i = 1;
  while(2*i <= *n_elem) // mientras tenga algun hijo
  {
    k = 2*i; //el hijo izquierdo
    if(k+1 <= *n_elem && ((Elem *)((char *)heap + ((k+1)-1)*pitch))[id].dist > ((Elem *)((char *)heap + (k-1)*pitch))[id].dist)
      k = k+1;  //el hijo derecho es el mayor

    if(((Elem *)((char *)heap + (i-1)*pitch))[id].dist > ((Elem *)((char *)heap + (k-1)*pitch))[id].dist)
      break;  //es mayor que ambos hijos

    temp.dist = ((Elem *)((char *)heap + (i-1)*pitch))[id].dist;
    temp.ind = ((Elem *)((char *)heap + (i-1)*pitch))[id].ind;
    ((Elem *)((char *)heap + (i-1)*pitch))[id].dist = ((Elem *)((char *)heap + (k-1)*pitch))[id].dist;
    ((Elem *)((char *)heap + (i-1)*pitch))[id].ind = ((Elem *)((char *)heap + (k-1)*pitch))[id].ind;
    ((Elem *)((char *)heap + (k-1)*pitch))[id].dist = temp.dist;
    ((Elem *)((char *)heap + (k-1)*pitch))[id].ind = temp.ind;
    i = k;   //lo intercambiamos con el mayor hijo
  }
  return;
//  return max;
}


__device__ double topH(Elem *heap, int id)
{
  return ((Elem *)((char *)heap + 0))[id].dist;
}

__device__ void popush(Elem *heap, Elem *elem, int *n_elem, int pitch, int id)
{
  int i, k;
  Elem temp;

  ((Elem *)((char *)heap+0))[id].dist = elem->dist;
  ((Elem *)((char *)heap+0))[id].ind  = elem->ind;

  i = 1;
  while(2*i <= *n_elem) // mientras tenga algun hijo
  {
    k = 2*i; //el hijo izquierdo
    if(k+1 <= *n_elem && ((Elem *)((char *)heap + ((k+1)-1)*pitch))[id].dist > ((Elem *)((char *)heap + (k-1)*pitch))[id].dist)
      k = k+1;  //el hijo derecho es el mayor

    if(((Elem *)((char *)heap + (i-1)*pitch))[id].dist > ((Elem *)((char *)heap + (k-1)*pitch))[id].dist)
      break;  //es mayor que ambos hijos

    temp.dist = ((Elem *)((char *)heap + (i-1)*pitch))[id].dist;
    temp.ind = ((Elem *)((char *)heap + (i-1)*pitch))[id].ind;
    ((Elem *)((char *)heap + (i-1)*pitch))[id].dist = ((Elem *)((char *)heap + (k-1)*pitch))[id].dist;
    ((Elem *)((char *)heap + (i-1)*pitch))[id].ind = ((Elem *)((char *)heap + (k-1)*pitch))[id].ind;
    ((Elem *)((char *)heap + (k-1)*pitch))[id].dist = temp.dist;
    ((Elem *)((char *)heap + (k-1)*pitch))[id].ind = temp.ind;
    i = k;   //lo intercambiamos con el mayor hijo
  }
  return;
}

__device__ double distancia_trans(double *p1, int pitch_p1, int col_1, double *q)
{
   int i=0;
   double suma=0;

   for (i=0; i < DIM; i++)
      suma += (((double *)((char *)p1 + (i*pitch_p1)))[col_1] - q[i]) * 
              (((double *)((char *)p1 + (i*pitch_p1)))[col_1] - q[i]);

   return sqrtf(suma);  
}

void imprime_trans(double **MAT, int col)
{
   int i;
   for (i=0; i<DIM; i++)
      printf("%lf ", MAT[i][col]);
   return;
}

int leedato(double *dato, FILE *file)
{
   int i=0;
   
   for (i=0;i<DIM;i++)
      if (fscanf(file,"%lf",&dato[i])<1)
         return -1;
   return 1;
}


int leedato_cophir(double *dato, FILE *file)
{
   int i=0;
   int num_f;
   
   for (i=0;i<DIM;i++)
   {
      if (fscanf(file, "%d", &num_f) < 1)
         return ERROR;
      dato[i] = (double)num_f;
      
      if (i+1 < DIM)
         if (fgetc(file) != ',')
         {
            printf("\nERROR :: ',' no encontrada\n");
            return ERROR;
         }
   }
   return 1;
}


int leedato_trans(double **dato, FILE *file, int col)
{
   int i=0;
   
     for (i=0;i<DIM;i++)
       if (fscanf(file,"%lf",&(dato[i][col]))<1)
         return -1;
   return 1;
}

int leedato_trans_cophir(double **dato, FILE *file, int col)
{
   int i=0;
   int num_f;
   
   for (i=0;i<DIM;i++)
   {
      if (fscanf(file, "%d", &num_f) < 1)
         return ERROR;
      dato[i][col] = (double)num_f;
      
      if (i+1 < DIM)
         if (fgetc(file) != ',')
         {
            printf("\nERROR :: ',' no encontrada\n");
            return ERROR;
         }
   }
   return 1;
}
    


__global__ void Batch_Heap_Reduction(double *DB_dev, int pitch_DB, Elem *heap, int pitch_H, double *QUERY_dev, int pitch_QUERY, Elem *arr_Dist, int pitch_Dist, int beginQ, double *res_final)
{
  int i, j, n_elem=0, n_elemWarp=0;
  int id;
  Elem eresult;
  __shared__ Elem matrizWarp[TOPK][TAM_WARP];
  __shared__ Elem heapfin[TOPK][1];
  __shared__ double query[DIM];
//  int ED=0;

  id = threadIdx.x + (blockDim.x * blockIdx.x);

  //Se copia la Query a mem. compartida
  for (i=threadIdx.x; i < DIM; i += blockDim.x)
      query[i] = ((double *)((char *)QUERY_dev + ((blockIdx.x + beginQ) * (int)pitch_QUERY)))[i];

  __syncthreads();

  //Se obtiene el arreglo de distancias
  for (i=threadIdx.x; i < NE; i += blockDim.x)
  {
//    ED++;
    ((Elem *)((char *)arr_Dist + (blockIdx.x*pitch_Dist)))[i].dist = distancia_trans(DB_dev, pitch_DB, i, query);
    ((Elem *)((char *)arr_Dist + (blockIdx.x*pitch_Dist)))[i].ind = i;
  }

  for(i=threadIdx.x; i < NE; i += blockDim.x)//NE = Numero de elementos de la BD
  {
      if (n_elem >= TOPK)
      {
         if (topH(heap, id) > ((Elem *)((char *)arr_Dist + (blockIdx.x*pitch_Dist)))[i].dist)
            popush(heap, &(((Elem *)((char *)arr_Dist + (blockIdx.x*pitch_Dist)))[i]), &n_elem, pitch_H, id); //Extrae e inserta en una operacion
      }
      else
         insertaH(heap, &(((Elem *)((char *)arr_Dist + (blockIdx.x*pitch_Dist)))[i]), &n_elem, pitch_H, id);
  }

  __syncthreads();


  //Un warp reduce el problema a una matriz de Kx32 distancias. PEro esta vez los heaps se almacenan en Memoria Shared
  if (threadIdx.x < TAM_WARP)
  {
    for(j=id; j < blockDim.x*(blockIdx.x+1); j += TAM_WARP)
    {
       n_elem = TOPK;
       for(i=0; i < TOPK; i++)
       {
         extraeH(heap, &n_elem, pitch_H, j, &eresult);

         if (n_elemWarp < TOPK)
           insertaH(&(matrizWarp[0][0]), &eresult, &n_elemWarp, sizeof(Elem)*TAM_WARP, threadIdx.x);
         else
           if (topH(&(matrizWarp[0][0]), threadIdx.x) > eresult.dist)
             popush(&(matrizWarp[0][0]), &eresult, &n_elemWarp, sizeof(Elem)*TAM_WARP, threadIdx.x);
       }
    }
  }
  

  __syncthreads();


  //Un hilo encuentra los K-NN a partir de la matriz de TOPKxTAM_WARP
  if (threadIdx.x == 0)
  {
     n_elem = 0;
     for(j=0; j < TAM_WARP; j++)
     {
       for(i=0; i < TOPK; i++)
          if (n_elem < TOPK)
             insertaH((Elem *)heapfin, &(matrizWarp[i][j]), &n_elem, sizeof(Elem), 0);
          else
             if (topH((Elem *)heapfin, 0) > matrizWarp[i][j].dist)
               popush((Elem *)heapfin, &(matrizWarp[i][j]), &n_elem, sizeof(Elem), 0);
     }

     //Escribiendo algunos resultados
     //res_final[blockIdx.x] = topH((Elem *)heapfin, 0);

     for (i=TOPK*blockIdx.x; i < (TOPK*blockIdx.x)+TOPK; i++)
     {
        extraeH(&(heapfin[0][0]), &n_elem, sizeof(Elem), 0, &eresult);
        res_final[i] = eresult.dist;
     }
  }

//  atomicAdd(&(resT[blockIdx.x]), ED);
  
  return;
}}