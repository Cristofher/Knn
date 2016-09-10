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

//para el calculo de tiempos
#include <sys/resource.h>
#include <time.h>
#include <sys/time.h>

//DIM es la dimensión de los objetos
//#define DIM 20
//TOPK es el k de las consultas kNN
//#define TOPK 16

#define ERROR -1

//Variables globales

int DIM;
int TOPK;

struct _Elem {
    float dist;
    int ind;
};
typedef struct _Elem Elem;

void busqueda(FILE *fquery, char **Centros, int *Rc, char **Clusters);
void inserta2(Elem *heap, Elem *elem, int *n_elem);
void extrae2(Elem *heap, int *n_elem, Elem *elem_extraido);
void popush2(Elem *heap, int *n_elem, Elem *elem);
float topH(Elem *heap, int *n_elem);
void shell_sort(int *arr, int MAX);
void copiavalor(float *a, float *b);
void imprime(float *a);
void imprimetodo(float **a,int b, int c);
float distancia(float *p1, float *p2);
int leedato(float *dato, FILE *file);
//La funcion leedato_cophir() es para ser usada con la BD CoPhIR
int leedato_cophir(float *dato, FILE *file);

int main(int argc, char *argv[]) {
    
    if (argc != 7) {
        printf("Error :: Ejecutar como : a.out archivo_BD Num_elem archivo_queries Num_queries N_THREADS\n");
        return 0;
    }
    
    TOPK = atoi(argv[5]);
    DIM = atoi(argv[6]);
    float **DB;
    float **Consultas; //Cola de consultas
    int N_QUERIES, N_DB;
    char str_f[256];
    float dato[DIM];
    int I, i, j;
    FILE *f_dist, *fquery;
    float real_time;
    struct timeval t1, t2;
    int encontrados = 0;
    int radio;
    Elem *heap, e_temp;
    //arr_K representa el arreglo con los K de las consultas que se quieren utilizar.
    int n_elem = 0;
    float d;
    double suma = 0;


    N_QUERIES = atoi(argv[4]);
    N_DB = atoi(argv[2]);

    sprintf(str_f, "%s", argv[1]);
    printf("\nAbriendo %s... ", argv[1]);
    fflush(stdout);
    f_dist = fopen(str_f, "r");
    printf("OK\n");
    fflush(stdout);

    printf("\nN_QUERIES = %d\n", N_QUERIES);
    fflush(stdout);

    Consultas = (float **) malloc(sizeof (float *)*N_QUERIES);
    for (i = 0; i < N_QUERIES; i++)
        Consultas[i] = (float *) malloc(sizeof (float)*DIM);

    DB = (float **) malloc(sizeof (float *)*N_DB);
    for (i = 0; i < N_DB; i++)
        DB[i] = (float *) malloc(sizeof (float)*DIM);

    printf("\nCargando DB... ");
    fflush(stdout);
    for (i = 0; i < N_DB; i++) {
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
        if (leedato(dato, fquery) == ERROR || feof(fquery)) {
            printf("\n\nERROR :: N_QUERIES mal establecido, Menos queries que las indicadas\n\n");
            fflush(stdout);
            fclose(fquery);
            break;
        }
        printf("\n");
        imprimetodo(DB,N_DB,DIM);
        copiavalor(Consultas[i], dato);
    }
    fclose(fquery);
    printf("OK\n");
    fflush(stdout);

    suma = 0;
    heap = (Elem *) malloc(sizeof (Elem) * TOPK);

    gettimeofday(&t1, 0);

    //Acceso Exhaustivo
    for (i = 0; i < N_QUERIES; i++) {
        n_elem = 0;
        for (j = 0; j < N_DB; j++) {
            printf("Num_elem:: %d\n",n_elem );
            d = distancia(Consultas[i], DB[j]);
            if (n_elem<TOPK){
                inserta2(heap, &e_temp, &n_elem);
            }
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
        //En este punto del código se tienen los K elemntos más cercanos a la consulta en 'heap'. Se peuden extraer con extraer2()
            printf("Consulta: %d de %d\n",i,TOPK );
            imprime(Consultas[i]);
            printf("\n");
            extrae2(heap, &n_elem, &e_temp);
        //Realizamos una operación con los resultados para que el compilador no evite hacer instrucciones que considere que el usuario no utiliza.
        suma += e_temp.dist;
    }
    gettimeofday(&t2, 0);


        for (j = 0; j < TOPK; j++) {

            printf("Vector K[%d]: ",j);
            printf("%d\n",heap[j].ind);
            printf("%lf\n",heap[j].dist );
            imprime(DB[heap[j].ind]);
            printf("%d ind = %d :: dist = %f\n", j, heap[j].ind, heap[j].dist);
        }

    real_time = (t2.tv_sec - t1.tv_sec) + (float) (t2.tv_usec - t1.tv_usec) / 1000000;
    printf("\nK = %d", TOPK);
    printf("\nReal Time = %f \n", real_time);
    printf("\nsuma = %lf \n", suma); //Esta variable suma se imprime solo para evitar que el compilador deje de ejecutar instrucciones que el usuario no utiliza, pero que necesitamos que igualmente las ejecute para medir el tiempo. Todas las instrucciones de las consultas knn las necesitamos para poder obtener la variable suma.
    fflush(stdout);
    free(heap);
    return 0;
}

void imprime(float *a) {
    int i;
    for (i = 0; i < DIM; i++)
        printf("%f ",a[i] );
    return;
}

void imprimetodo(float **a,int N_DB,int DIM){
    int i,j;
    for (i = 0; i < N_DB; ++i){
     for (j = 0; j < DIM; ++j)
     {
         printf("%f _ ",a[i][j]);
     }
     printf("\n");
    }
}

void copiavalor(float *a, float *b) {
    int i;
    for (i = 0; i < DIM; i++)
        a[i] = b[i];
    return;
}

float distancia(float *p1, float *p2) {
    int i = 0;
    float suma = 0;

    for (i = 0; i < DIM; i++)
        suma += ((p1[i] - p2[i])*(p1[i] - p2[i]));
    //    return ((float)sqrt((double)suma));
    return sqrtf(suma);
}

int leedato(float *dato, FILE *file) {
    int i = 0;

    for (i = 0; i < DIM; i++)
        if (fscanf(file, "%f", &dato[i]) < 1)
            return ERROR;
    return 1;
}
 

int leedato_cophir(float *dato, FILE *file) {
    int i = 0;
    int num_f;

    for (i = 0; i < DIM; i++) {
        if (fscanf(file, "%d", &num_f) < 1)
            return ERROR;

        dato[i] = (float) num_f;
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

float topH(Elem *heap, int *n_elem) {
    if (*n_elem == 0)
        return MAXFLOAT;
    return heap[0].dist;
}



