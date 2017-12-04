#include <papi.h>
#include <pthread.h>


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


#include "testlib/papi_test.h"
const PAPI_hw_info_t *hw_info = NULL;


#define INDEX 1000
#define OVER_FMT	"handler(%d ) Overflow at %p! bit=%#llx \n"
#define OUT_FMT		"%-12s : %16lld%16lld\n"


char format_string[] =
{ "Real_time: %f Proc_time: %f Total flpins: %lld MFLOPS: %f\n" };
extern int TESTS_QUIET;                /* Declared in test_utils.c */

static int total = 0;



#define NUM_EVENTS 3
long long values[NUM_EVENTS];
unsigned int Events[NUM_EVENTS]={PAPI_TOT_INS,PAPI_TOT_CYC,PAPI_L1_DCA};
int mat=0;

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

void handler( int EventSet, void *address, long long overflow_vector, void *context ){
	( void ) context;
	if ( !TESTS_QUIET ) {
		fprintf( stderr, OVER_FMT, EventSet, address, overflow_vector );
	}
	total++;
}

void Thread( int n ){

	int retval, num_tests = 1;
	int EventSet1 = PAPI_NULL;
	int PAPI_event, mask1;
	int num_events1;
	long long **values;
	long long elapsed_us, elapsed_cyc, L1_DCM;
	char event_name[PAPI_MAX_STR_LEN];

	printf( "Thread %#x started\n", omp_get_thread_num(  ) );
	num_events1 = 2;

	EventSet1 = add_two_events( &num_events1, &PAPI_event, &mask1 );

	retval = PAPI_event_code_to_name( PAPI_event, event_name );
	if ( retval != PAPI_OK )
		test_fail( __FILE__, __LINE__, "PAPI_event_code_to_name", retval );

	values = allocate_test_space( num_tests, num_events1 );

	elapsed_us = PAPI_get_real_usec(  );
	
	elapsed_cyc = PAPI_get_real_cyc(  );

	retval = PAPI_start( EventSet1 );

	do_flops( n );

	retval = PAPI_stop( EventSet1, values[0] );

	elapsed_us = PAPI_get_real_usec(  ) - elapsed_us;

	elapsed_cyc = PAPI_get_real_cyc(  ) - elapsed_cyc;

	remove_test_events( &EventSet1, mask1 );

	if ( !TESTS_QUIET ) {
		printf( "Thread %#x %-12s : \t%lld\n", omp_get_thread_num(  ),
			event_name, values[0][1] );
		printf( "Thread %#x PAPI_TOT_CYC: \t%lld\n", omp_get_thread_num(  ),
			values[0][0] );
		printf( "Thread %#x Real usec   : \t%lld\n", omp_get_thread_num(  ),
			elapsed_us );
		printf( "Thread %#x Real cycles : \t%lld\n", omp_get_thread_num(  ),
			elapsed_cyc );
	}

	free_test_space( values, num_tests );

	PAPI_unregister_thread(  );
	printf( "Thread %#x finished\n", omp_get_thread_num(  ) );
}

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


	float rtime1, rtime2, ptime1, ptime2, mflops;
	long long flpops;

	unsigned long int tid;
	int num_hwcntrs = 0;
	int fip = 0, retval;
	float real_time, proc_time;
	long long flpins;

	int i;
	unsigned int EventSet = PAPI_NULL; 
    int count = 0, err_count = 0;


    PAPI_event_info_t info;

    long long ( values2[2] )[2];
    long long min, max;
    int PAPI_event, mythreshold = THRESHOLD;
    char event_name1[PAPI_MAX_STR_LEN];
    const PAPI_hw_info_t *hw_info = NULL;
    int num_events, mask;
    int num_flops = NUM_FLOPS;
    long long elapsed_us, elapsed_cyc;



tests_quiet( argc, argv );  /* Set TESTS_QUIET variable */



    retval = PAPI_library_init( PAPI_VER_CURRENT );
    if ( retval != PAPI_VER_CURRENT )
      test_fail( __FILE__, __LINE__, "PAPI_library_init", retval );

  retval = PAPI_create_eventset( &EventSet );
  if ( retval != PAPI_OK )
      test_fail( __FILE__, __LINE__, "PAPI_create_eventset", retval );

	/* Get hardware info */
  hw_info = PAPI_get_hardware_info(  );
  if ( hw_info == NULL )
      test_fail( __FILE__, __LINE__, "PAPI_get_hardware_info", 2 );

  EventSet = 	add_two_nonderived_events( &num_events, &PAPI_event, &mask );

  printf("Using %#x for the overflow event\n",PAPI_event);

  if ( PAPI_event == PAPI_FP_INS ) {
      mythreshold = THRESHOLD;
  }
  else {
		#if defined(linux)
      mythreshold = ( int ) hw_info->cpu_max_mhz * 20000;
		#else
      mythreshold = THRESHOLD * 2;
		#endif
  }

  retval = PAPI_start( EventSet );
  if ( retval != PAPI_OK )
      test_fail( __FILE__, __LINE__, "PAPI_start", retval );

  do_flops( NUM_FLOPS );

	/* stop the calibration run */
  retval = PAPI_stop( EventSet, values2[0] );
  if ( retval != PAPI_OK )
      test_fail( __FILE__, __LINE__, "PAPI_stop", retval );


	/* set up overflow handler */
  retval = PAPI_overflow( EventSet, PAPI_event, mythreshold, 0, handler );
  if ( retval != PAPI_OK ) {
      test_fail( __FILE__, __LINE__, "PAPI_overflow", retval );
  }

	/* Start overflow run */
  retval = PAPI_start( EventSet );
  if ( retval != PAPI_OK ) {
      test_fail( __FILE__, __LINE__, "PAPI_start", retval );
  }

  do_flops( num_flops );

	/* stop overflow run */
  retval = PAPI_stop( EventSet, values2[1] );
  if ( retval != PAPI_OK )
      test_fail( __FILE__, __LINE__, "PAPI_stop", retval );
  retval = PAPI_overflow( EventSet, PAPI_event, 0, 0, handler );
  if ( retval != PAPI_OK )
      test_fail( __FILE__, __LINE__, "PAPI_overflow", retval );

  if ( !TESTS_QUIET ) {
      if ( ( retval =
         PAPI_event_code_to_name( PAPI_event, event_name1 ) ) != PAPI_OK )
         test_fail( __FILE__, __LINE__, "PAPI_event_code_to_name", retval );

     printf( "Test case: Overflow dispatch of 2nd event in set with 2 events.\n" );
     printf( "---------------------------------------------------------------\n" );
     printf( "Threshold for overflow is: %d\n", mythreshold );
     printf( "Using %d iterations\n", num_flops );
     printf( "-----------------------------------------------\n" );

     printf( "Test type    : %16d%16d\n", 1, 2 );
     printf( OUT_FMT, event_name1, ( values2[0] )[1], ( values2[1] )[1] );
     printf( OUT_FMT, "PAPI_TOT_CYC", ( values2[0] )[0], ( values2[1] )[0] );
     printf( "Overflows    : %16s%16d\n", "", total );
     printf( "-----------------------------------------------\n" );
 }

 retval = PAPI_cleanup_eventset( EventSet );
 if ( retval != PAPI_OK )
  test_fail( __FILE__, __LINE__, "PAPI_cleanup_eventset", retval );

retval = PAPI_destroy_eventset( &EventSet );
if ( retval != PAPI_OK )
  test_fail( __FILE__, __LINE__, "PAPI_destroy_eventset", retval );

if ( !TESTS_QUIET ) {
  printf( "Verification:\n" );
#if defined(linux) || defined(__ia64__) || defined(_POWER4)
  num_flops *= 2;
#endif
  if ( PAPI_event == PAPI_FP_INS || PAPI_event == PAPI_FP_OPS ) {
     printf( "Row 1 approximately equals %d %d\n", num_flops, num_flops );
 }
 printf( "Column 1 approximately equals column 2\n" );
 printf( "Row 3 approximately equals %u +- %u %%\n",( unsigned ) ( ( values2[0] )[1] / ( long long ) mythreshold ),( unsigned ) ( OVR_TOLERANCE * 100.0 ) );
}

min =
( long long ) ( ( ( double ) values2[0][1] * ( 1.0 - OVR_TOLERANCE ) ) /
  ( double ) mythreshold );
max =
( long long ) ( ( ( double ) values2[0][1] * ( 1.0 + OVR_TOLERANCE ) ) /
  ( double ) mythreshold );
printf( "Overflows: total(%d) > max(%lld) || total(%d) < min(%lld) \n", total,
  max, total, min );
if ( total > max || total < min )
  test_fail( __FILE__, __LINE__, "Overflows", 1 );



printf("Initial thread id is: %lu\n",tid);

	/* Initialize the PAPI library and get the number of counters available */

if ((num_hwcntrs = PAPI_num_counters()) <= 0)  
  handle_error(1);



  /*  The installation supports PAPI, but has no counters */
if ((num_hwcntrs = PAPI_num_counters()) == 0 )
    fprintf(stderr,"Info:: This machine does not provide hardware counters.");

printf("This system has %d available counters.\n", num_hwcntrs);

if (num_hwcntrs > 2)
  num_hwcntrs = 2;

	 /* Start counting events */




if (PAPI_start_counters(Events, num_hwcntrs) != PAPI_OK)
  handle_error(1);

if (argc != 8) {
  printf("\nError :: Ejecutar como : a.out archivo_BD Num_elem archivo_queries Num_queries N_THREADS numero_K Dimension_objetos\n");
  return 0;
}
TOPK = atoi(argv[6]);
DIM = atoi(argv[7]);
double **DB;
	double **Consultas; //Cola de consultas
	int N_QUERIES, N_DB;
	char str_f[256];
	double dato[DIM];
	int j;
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

	PAPI_start_counters((int*) Events, NUM_EVENTS);
	omp_set_num_threads(N_THREADS);

	elapsed_us = PAPI_get_real_usec(  );

	elapsed_cyc = PAPI_get_real_cyc(  );

	retval =
	PAPI_thread_init( ( unsigned
		long ( * )( void ) ) ( omp_get_thread_num ) );
	if ( retval != PAPI_OK ) {
		if ( retval == PAPI_ECMP )
			test_skip( __FILE__, __LINE__, "PAPI_thread_init", retval );
		else
			test_fail( __FILE__, __LINE__, "PAPI_thread_init", retval );
	}

#pragma omp parallel shared(Consultas, DB, N_QUERIES, N_DB, N_THREADS, acum, DIM)
	{
		float real_time;
		struct timeval t1, t2;
		int i, j;
		Elem *heap, e_temp;
		double d;
		int n_elem = 0;
		int trid = omp_get_thread_num(); //ID del thread
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
        for (i = trid; i < N_QUERIES; i += procs) {
         n_elem = 0;
         for (j = 0; j < N_DB; j++) {

            d = distancia(Consultas[i], DB[j]);
				//Si la distancia del objeto a la consulta es menor que la raíz del heap, entonces se inserta en el heap. La raíz siempre mantiene la mayor de las distancias

            if(n_elem<TOPK){
               e_temp.dist = d;
               e_temp.ind = j;
               inserta2(heap, &e_temp, &n_elem);
           }
           if (n_elem==TOPK){
               if (d < topH(heap, &n_elem)) {
                  e_temp.dist = d;
                  e_temp.ind = j;
					//Si el heap no está lleno, se inserta el elemento
                  if (n_elem < TOPK)
                     inserta2(heap, &e_temp, &n_elem);
						//Si el heap está lleno, se inserta el elemento nuevo y se saca el que era antes de mayor de distancia. popush2() hace las operaciones de sacar el elemento mayor e insertar el nuevo.
                 else
                     popush2(heap, &n_elem, &e_temp);
             }}
         }

			//En este punto del código se tienen los K elemntos más cercanos a la consulta en 'heap'. Se pueden extraer con extraer2()
         for (j = 0; j < TOPK ; j++) {
           extrae2(heap, &n_elem, &e_temp);
           answer[i*TOPK+j].ind = e_temp.ind;
           answer[i*TOPK+j].dist = e_temp.dist;
       }
			//Realizamos una operación con los resultados para que el compilador no evite hacer instrucciones que considere que el usuario no utiliza. Simplemente cada hilo suma las distancias de los elementos mas cercanos a la consulta 
   }
   Thread( 1000000 * ( tid + 1 ) );



   fflush(stdout);

#pragma omp barrier

#pragma omp master
   {   

    if ( fip > 0 ) {
		/* Setup PAPI library and begin collecting data from the counters */
       if ( fip == 1 ) {
          if ( ( retval =
             PAPI_flips( &real_time, &proc_time, &flpins,
                &mflops ) ) < PAPI_OK )
             test_fail( __FILE__, __LINE__, "PAPI_flips", retval );
     } else {
      if ( ( retval =
         PAPI_flops( &real_time, &proc_time, &flpins,
            &mflops ) ) < PAPI_OK )
         test_fail( __FILE__, __LINE__, "PAPI_flops", retval );
 }

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


if ( fip == 1 ) {
  if ( ( retval =
     PAPI_flips( &real_time, &proc_time, &flpins,
        &mflops ) ) < PAPI_OK )
     test_fail( __FILE__, __LINE__, "PAPI_flips", retval );
} else {
  if ( ( retval =
     PAPI_flops( &real_time, &proc_time, &flpins,
        &mflops ) ) < PAPI_OK )
     test_fail( __FILE__, __LINE__, "PAPI_flops", retval );
}

if ( !TESTS_QUIET ) {
  if ( fip == 1 ) {
     printf( "Real_time: %f Proc_time: %f Total flpins: ", real_time,
        proc_time );
 } else {
     printf( "Real_time: %f Proc_time: %f Total flpops: ", real_time,
        proc_time );
 }
 printf( LLDFMT, flpins );
 printf( " MFLOPS: %f\n", mflops );
}
}

}
free(heap);



	}//end pragma omp parallel

	elapsed_cyc = PAPI_get_real_cyc(  ) - elapsed_cyc;
	elapsed_us = PAPI_get_real_usec(  ) - elapsed_us;

	if ( !TESTS_QUIET ) {
		printf( "Master real usec   : \t%lld\n", elapsed_us );
		printf( "Master real cycles : \t%lld\n", elapsed_cyc );
	}

	const PAPI_hw_info_t *hwinfo = NULL;
	const PAPI_mh_tlb_info_t *mhinfo = NULL;
	const  PAPI_mh_cache_info_t *mhcacheinfo = NULL;
	const PAPI_mh_level_t *mhlevel = NULL;


	if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
		exit(1);
	if ((hwinfo = PAPI_get_hardware_info()) == NULL)
		exit(1);	
	if ((mhinfo = PAPI_get_hardware_info()) == NULL)
		exit(1);
	if ((mhcacheinfo = PAPI_get_hardware_info()) == NULL)
		exit(1);
	if ((mhlevel = PAPI_get_hardware_info()) == NULL)
		exit(1);

	printf("\n\nA continuación información actual del equipo\n\n");

	printf("MH Type %d - Num entries %d  - Associativity %d \n",mhinfo->type, mhinfo->num_entries, mhinfo->associativity);
	printf("Cache MH type %d size %d line size %d num_lines %d Associativity %d\n\n",mhcacheinfo->type, mhcacheinfo->size,mhcacheinfo->line_size, mhcacheinfo->num_lines, mhcacheinfo->associativity);



    retval=papi_print_header("Available PAPI preset and user defined events plus hardware information.\n",&hwinfo );


    printf("Total hardware flops = %lld\n",(float)values[1]);
    printf("L2 data cache misses is %lld\n", values[0]);






    retval = PAPI_stop_counters(values, NUM_EVENTS);
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



