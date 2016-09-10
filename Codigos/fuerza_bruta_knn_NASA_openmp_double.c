/*
 <<Copyright 2010 Ricardo J. Barrientos>>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */




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


//DIM es la dimensión de los objetos
//#define DIM 20

//TOPK es el K de las consultas kNN
//#define TOPK 32

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
    Elem *heap, e_temp;
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
#pragma omp parallel shared(Consultas, DB, N_QUERIES, N_DB, N_THREADS, acum)
    {
        float real_time;
        struct timeval t1, t2;
        int i, j;
        Elem *heap, e_temp;
        int d, n_elem = 0;
        int tid = omp_get_thread_num(); //ID del thread
        int procs = omp_get_num_threads(); //Nro. total de threads
        double suma = 0;

        suma = 0;
        heap = (Elem *) malloc(sizeof (Elem) * TOPK);

#pragma omp barrier

#pragma omp master
        {
            gettimeofday(&t1, 0);
        }

        //Cada hilo accede a un subconjunto de las consultas. Cada hio accede de manera circular al arreglo de consultas.
        for (i = tid; i < N_QUERIES; i += procs) {
            n_elem = 0;
            for (j = 0; j < N_DB; j++) {
                d = distancia(Consultas[i], DB[j]);
                //Si la distancia del objeto a la consulta es menor que la raíz del heap, entonces se inserta en el heap. La raíz siempre mantiene la mayor de las distancias
                if (d < topH(heap, &n_elem)) {
                    e_temp.dist = d;
                    e_temp.ind = j;
                    //Si el heap no está lleno, se inserta el elemento
                    if (n_elem < TOPK)
                        inserta2(heap, &e_temp, &n_elem);
                        //Si el heap está lleno, se inserta el elemento nuevo y se saca el que era antes de mayor de distancia. popush2() hace las operaciones de sacar el elemento mayor e insertar el nuevo.
                    else
                        popush2(heap, &n_elem, &e_temp);
                }
            }
            //En este punto del código se tienen los K elemntos más cercanos a la consulta en 'heap'. Se pueden extraer con extraer2()

            for (j = 0; j < TOPK && n_elem > 0; j++) {
                extrae2(heap, &n_elem, &e_temp);
                //  printf("%d) ind = %d :: dist = %d\n", j, e_temp.ind, e_temp.dist);
            }
            //Realizamos una operación con los resultados para que el compilador no evite hacer instrucciones que considere que el usuario no utiliza. Simplemente cada hilo suma las distancias de los elementos mas cercanos a la consulta 
            acum[tid] += e_temp.dist;

        }

#pragma omp barrier

#pragma omp master
        {
            gettimeofday(&t2, 0);
            real_time = (t2.tv_sec - t1.tv_sec) + (float) (t2.tv_usec - t1.tv_usec) / 1000000;

            for (i = 0; i < N_THREADS; i++)
                printf("\nacum[%d] = %d", tid, acum[i]);
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
    //    return ((float)sqrt((double)suma));
    return sqrtf(suma);
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

//inserta2 y extrae2 usan el arreglo del heap desde el elemento 0

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

//inserta2 y extrae2 usan el arreglo del heap desde el elemento 0

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



