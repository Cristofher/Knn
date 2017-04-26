#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <values.h>
#include <math.h>
#include <omp.h>
//para el calculo de tiempos
#include <sys/resource.h>
#include <time.h>
#include <sys/time.h>

// Global variables
int TOPK;
int DIM;
FILE *Salida_Multihilo;

#define ERROR -1

struct _Elem {
    double dist;
    int ind;
};
typedef struct _Elem Elem;

void busqueda(FILE *fquery, char **Centros, int *Rc, char **Clusters);
void inserta2(Elem *heap, Elem *elem, int *n_elem);
void extrae2(Elem *heap, int *n_elem, Elem *elem_extraido);
void popush2(Elem *heap, int *n_elem, Elem *elem);
double topH(Elem *heap, int *n_elem);
void shell_sort(int *arr, int MAX);
void copiavalor(double *a, double *b);
double distancia(double *p1, double *p2);
int leedato(double *dato, FILE *file);
int leedato_cophir(double *dato, FILE *file);

int main(int argc, char *argv[]) {
    if (argc != 8) {
        printf("Error :: Ejecutar como : a.out archivo_BD Num_elem archivo_queries Num_queries N_THREADS numero_K Dimension_objetos\n");
        return 0;
    }
    TOPK = atoi(argv[6]);
    DIM = atoi(argv[7]);
    double **DB;
    double **Consultas; //Cola de consultas
    int N_QUERIES, N_DB;
    char str_f[256];
    double dato[DIM];
    int i, j;
    FILE *f_dist, *fquery;
    Elem *heap, e_temp,*answer;
    int *acum, N_THREADS;

    //N_THREADS es el nro. de threads con el que se lanzará la región paralela
    N_THREADS = atoi(argv[5]);
    //N_QUERIES es el nro. de consultas
    N_QUERIES = atoi(argv[4]);
    N_DB = atoi(argv[2]);

    printf("\nN_QUERIES = %d\nN_THREADS = %d\n", N_QUERIES, N_THREADS);
    fflush(stdout);

    acum = (int *) malloc(sizeof (int)*N_THREADS);
    for (i = 0; i < N_THREADS; i++)
        acum[i] = 0;

    sprintf(str_f, "%s", argv[1]);
    printf("\nAbriendo %s... ", argv[1]);
    fflush(stdout);
    f_dist = fopen(str_f, "r");
    printf("OK\n");
    fflush(stdout);


    Consultas = (double **) malloc(sizeof (double *)*N_QUERIES);
    for (i = 0; i < N_QUERIES; i++)
        Consultas[i] = (double *) malloc(sizeof (double)*DIM);

    DB = (double **) malloc(sizeof (double *)*N_DB);
    for (i = 0; i < N_DB; i++)
        DB[i] = (double *) malloc(sizeof (double)*DIM);

    answer = (Elem *)malloc(sizeof(Elem)*N_QUERIES*TOPK);

    printf("\nCargando DB... ");
    fflush(stdout);
    for (i = 0; i < N_DB; i++) {
        //Usar leedato_cophir() cuando se utilice la BD Cophir para no tener problemas con las ","
        //if (leedato_cophir(dato, f_dist) == ERROR || feof(f_dist))
        if (leedato(dato, f_dist) == ERROR || feof(f_dist)) {
            printf("\n\nERROR :: N_DB mal establecido\n\n");
            fflush(stdout);
            fclose(f_dist);
            break;
        }
        copiavalor(DB[i], dato);
    }
    fclose(f_dist);
    printf("OK\n");
    fflush(stdout);

    if ((fquery = fopen(argv[3], "r")) == NULL)
        printf("Error al abrir para lectura el archivo de qeuries: %s\n", argv[3]);
    else
        printf("Abriendo  para lectura %s\n", argv[3]);
    printf("\nCargando Consultas... ");
    fflush(stdout);
    for (i = 0; i < N_QUERIES; i++) {
        //Usar leedato_cophir() cuando se utilice la BD Cophir para no tener problemas con las ","
        //if (leedato_cophir(dato, fquery) == ERROR || feof(fquery))
        if (leedato(dato, fquery) == ERROR || feof(fquery)) {
            printf("\n\nERROR :: N_QUERIES mal establecido, Menos queries que las indicadas\n\n");
            fflush(stdout);
            fclose(fquery);
            break;
        }
        copiavalor(Consultas[i], dato);
    }
    fclose(fquery);
    printf("OK\n");
    fflush(stdout);



omp_set_num_threads(N_THREADS);
#pragma omp parallel shared(Consultas, DB, N_QUERIES, N_DB, N_THREADS, acum, DIM)
    {

->Funcion<-


#pragma omp master
    {
        gettimeofday(&t2, 0);
        real_time = (t2.tv_sec - t1.tv_sec) + (float) (t2.tv_usec - t1.tv_usec) / 1000000;

        Salida_Multihilo = fopen("Salida_Multihilo.txt", "w");
        for (i = 0; i < N_QUERIES; ++i){
          fprintf(Salida_Multihilo, "Consulta id:: %d\n",i);
          for (j = 0; j < TOPK; ++j){
            fprintf(Salida_Multihilo,"ind = %d :: dist = %f\n",answer[(i*TOPK)+j].ind,answer[(i*TOPK)+j].dist);
        }
        fprintf(Salida_Multihilo, "---------------------------------\n");
    }
    fclose(Salida_Multihilo);

    printf("\n\nK = %d", TOPK);
    printf("\nReal Time = %f segundos.\n", real_time);
    fflush(stdout);
}
free(heap);

    }//end pragma omp parallel

    return 0;
}

void copiavalor(double *a, double *b) {
    int i;
    for (i = 0; i < DIM; i++)
        a[i] = b[i];
    return;
}

double distancia(double *p1, double *p2) {
    int i = 0;
    double suma = 0;

    for (i = 0; i < DIM; i++)
        suma += ((p1[i] - p2[i])*(p1[i] - p2[i]));
    return sqrt(suma);
}

int leedato(double *dato, FILE *file) {
    int i = 0;

    for (i = 0; i < DIM; i++)
        if (fscanf(file, "%lf", &dato[i]) < 1)
            return ERROR;
        return 1;
    }

    int leedato_cophir(double *dato, FILE *file) {
        int i = 0;
        int num_f;

        for (i = 0; i < DIM; i++) {
            if (fscanf(file, "%d", &num_f) < 1)
                return ERROR;

            dato[i] = (double) num_f;
            if (i + 1 < DIM)
                if (fgetc(file) != ',') {
                    printf("\nERROR :: ',' no encontrada\n");
                    return ERROR;
                }
            }
            return 1;
        }

        void inserta2(Elem *heap, Elem *elem, int *n_elem) {
            int i;
            Elem temp;

            heap[*n_elem].dist = elem->dist;
            heap[*n_elem].ind = elem->ind;
            (*n_elem)++;
            for (i = *n_elem; i > 1 && heap[i - 1].dist > heap[(i / 2) - 1].dist; i = i / 2) {
        //Intercambiamos con el padre
                temp = heap[i - 1];
                heap[i - 1] = heap[(i / 2) - 1];
                heap[(i / 2) - 1] = temp;
            }
        }

        void extrae2(Elem *heap, int *n_elem, Elem *elem_extraido) {
            int i, k;
            Elem temp;

            (*elem_extraido).dist = heap[0].dist;
            (*elem_extraido).ind = heap[0].ind;

    heap[0] = heap[(*n_elem) - 1]; // Movemos el ultimo a la raiz y achicamos el heap
    (*n_elem)--;
    i = 1;
    while (2 * i <= *n_elem) // mientras tenga algun hijo
    {
        k = 2 * i; //el hijo izquierdo
        if (k + 1 <= *n_elem && heap[(k + 1) - 1].dist > heap[k - 1].dist)
            k = k + 1; //el hijo derecho es el mayor
        if (heap[i - 1].dist > heap[k - 1].dist)
            break; //es mayor que ambos hijos

        temp = heap[i - 1];
        heap[i - 1] = heap[k - 1];
        heap[k - 1] = temp;
        i = k; //lo intercambiamos con el mayor hijo
    }
    return;
}

void popush2(Elem *heap, int *n_elem, Elem *elem) {
    int i, k;
    Elem temp;

    heap[0].dist = elem->dist;
    heap[0].ind = elem->ind;

    i = 1;
    while (2 * i <= *n_elem) // mientras tenga algun hijo
    {
        k = 2 * i; //el hijo izquierdo
        if (k + 1 <= *n_elem && heap[(k + 1) - 1].dist > heap[k - 1].dist)
            k = k + 1; //el hijo derecho es el mayor
        if (heap[i - 1].dist > heap[k - 1].dist)
            break; //es mayor que ambos hijos

        temp = heap[i - 1];
        heap[i - 1] = heap[k - 1];
        heap[k - 1] = temp;
        i = k; //lo intercambiamos con el mayor hijo
    }
    return;
}

double topH(Elem *heap, int *n_elem) {
    if (*n_elem == 0)
        return MAXFLOAT;
    return heap[0].dist;
}



