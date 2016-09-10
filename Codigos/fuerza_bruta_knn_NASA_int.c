#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <values.h>
#include <math.h>
//para el calculo de tiempos
#include <sys/resource.h>
#include <time.h>
#include <sys/time.h>

// biblioteca que contiene las funciones de entrada y salida de c++ . Viene Input/Output Stream
#include <iostream> 

int DIM;
int TOPK;


#define ERROR -1

struct _Elem {
    double dist;
    double ind;
};
typedef struct _Elem Elem;

void busqueda(FILE *fquery, char **Centros, int *Rc, char **Clusters);
void inserta2(Elem *heap, Elem *elem, int *n_elem);
void extrae2(Elem *heap, int *n_elem, Elem *elem_extraido);
void popush2(Elem *heap, int *n_elem, Elem *elem);
double topH(Elem *heap, int *n_elem);
void shell_sort(int *arr, int MAX);
void copiavalor(int *a, int *b);
double distancia(double *p1, double *p2);
double leedato(double *dato, FILE *file);
double leedato_cophir(double *dato, FILE *file);

int main(int argc, char *argv[]) {
    
    if (argc != 7) {
        printf("Error :: Ejecutar como : a.out archivo_BD Num_elem archivo_queries Num_queries N_THREADS\n");
        return 0;
    }

    TOPK = atoi(argv[5]);
    DIM = atoi(argv[6]);
    int **DB;
    int **Consultas; //Cola de consultas
    int N_QUERIES, N_DB;
    char str_f[256];
    int dato[DIM];
    int I, i, j;
    FILE *f_dist, *fquery;
    float real_time;
    struct timeval t1, t2;
    int encontrados = 0;
    int radio;
    Elem *heap, e_temp;
    //arr_K representa el arreglo con los K de las consultas que se quieren utilizar.
    int n_elem = 0;
    double d;
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

    Consultas = (int **) malloc(sizeof (int *)*N_QUERIES);
    for (i = 0; i < N_QUERIES; i++)
        Consultas[i] = (int *) malloc(sizeof (int)*DIM);

    DB = (int **) malloc(sizeof (int *)*N_DB);
    for (i = 0; i < N_DB; i++)
        DB[i] = (int *) malloc(sizeof (int)*DIM);

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
        copiavalor(Consultas[i], dato);
    }
    fclose(fquery);
    printf("OK\n");
    fflush(stdout);

    suma = 0;
    heap = (Elem *) malloc(sizeof (Elem) * TOPK);

    gettimeofday(&t1, 0);

    printf("%lf \n",heap[1].dist );
    printf("%lf \n",heap[0].dist );
    printf("%lf \n",heap[2].dist );

    //Acceso Exhaustivo
    for (i = 0; i < N_QUERIES; i++) {
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
        //En este punto del código se tienen los K elemntos más cercanos a la consulta en 'heap'. Se peuden extraer con extraer2()

        for (j = 0; j < TOPK && n_elem > 0; j++) {
            extrae2(heap, &n_elem, &e_temp);
            printf("%d ind = %d :: dist = %lf\n", j, e_temp.ind, e_temp.dist);
        }
        //Realizamos una operación con los resultados para que el compilador no evite hacer instrucciones que considere que el usuario no utiliza.
        suma += e_temp.dist;
    }
    gettimeofday(&t2, 0);

    real_time = (t2.tv_sec - t1.tv_sec) + (float) (t2.tv_usec - t1.tv_usec) / 1000000;
    printf("\nK = %d", TOPK);
    printf("\nReal Time = %f \n", real_time);
    fflush(stdout);
    free(heap);
    return 0;
}

void copiavalor(int *a, int *b) {
    int i;
    for (i = 0; i < DIM; i++)
        a[i] = b[i];
    return;
}

double distancia(int *p1, int *p2) {
    int i = 0;
    int suma = 0;

    for (i = 0; i < DIM; i++)
        suma += ((p1[i] - p2[i])*(p1[i] - p2[i]));
    //    return ((double)sqrt((double)suma));
    return sqrtf(suma);
}

double leedato(double *dato, FILE *file) {
    int i = 0;

    for (i = 0; i < DIM; i++)
        if (cin >> dato[i]) < 1)
            return ERROR;
    return 1;
}

double leedato_cophir(double *dato, FILE *file) {
    int i = 0;
    double num_f;

    for (i = 0; i < DIM; i++) {
        if (cin >> num_f < 1)
            return ERROR;

        dato[i] = (int) num_f;
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
        return MAXDOUBLE;
    return heap[0].dist;
}



