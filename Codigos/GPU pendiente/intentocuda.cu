#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <string.h>

//To measure time:
#include <sys/resource.h>
#include <time.h>
#include <sys/time.h>

#define ERROR -1
#define LENGTH_ARRAY 100000 
#define TOPK 32 
#define TAM_WARP 32 
#define N_BLOQUES 3 
#define T_per_BLOCK 512 

// Global variables
int DIM;
FILE *Salida_Multihilo;

struct _Elem{
	float dist;
	int ind;
};
typedef struct _Elem Elem;


void copiavalor(double *a, double *b);
int leedato(double *dato, FILE *file);
__device__ void pushH(Elem *heap, Elem *elem, int *n_elem, int pitch, int id);
__device__ void popH(Elem *heap, int *n_elem, int pitch, int id, Elem *eresult);
__device__ float topH(Elem *heap, int id);
__device__ void popushH(Elem *heap, Elem *elem, int *n_elem, int pitch, int id);
__global__ void Batch_Heap_Reduction(Elem *heap, int pitch_H, Elem *arr_Dist, int pitch_Dist, Elem *res_final);


main(int argc, char *argv[])
{
	int i, j,N_QUERIES,N_DB;
	
	char *ruta_db, *ruta_queries;
	double **DB, **Consultas,*CudaDB, *CudaConsultas;
	size_t pitch_DB, pitch_Consultas;

	
	if (argc != 6){
		printf("Error :: Ejecutar como : main.out archivo_BD Num_elem archivo_queries Num_queries dim\n");
		return 1;
	}
	
	ruta_db = (char *)malloc(sizeof(char)*(strlen(argv[1])+1));
	strcpy(ruta_db, argv[1]);
	N_DB = atoi(argv[2]);

	printf("%s\n",ruta_db );

	ruta_queries = (char *)malloc(sizeof(char)*(strlen(argv[3])+1));
	strcpy(ruta_queries, argv[3]);
	N_QUERIES = atoi(argv[4]);
	printf("N_QUERIES:: -> :: %d\n",N_QUERIES );
	DIM = atoi(argv[5]);

	printf("dim:: %d\n",DIM );

	Elem *res_final, *res_final_H;
	Elem *HEAPS_dev, *arr_Dist, **arr_Dist_H;
	size_t pitch_H, pitch_Dist;
	//Variable for time:
	struct rusage r1, r2;
	float user_time, sys_time, real_time;
	struct timeval t1, t2;
	FILE *f_dist, *fquery;
	double dato[DIM],datos[DIM];

	//Lectura de Base de datos
	
	if (cudaSuccess != cudaMallocPitch((void **)&CudaDB, &pitch_DB, DIM * sizeof(int), N_DB)){
		printf("\nERROR :: cudaMallocPitch :: CudaDB\n");
		cudaThreadExit();
		return 0;
	}

	if (cudaSuccess != cudaMallocPitch((void **)&CudaConsultas, &pitch_Consultas, DIM * sizeof(int), N_QUERIES)){
		printf("\nERROR :: cudaMallocPitch :: CudaConsultas\n");
		cudaThreadExit();
		return 0;
	}

	printf("\nAbriendo %s... ", argv[1]);
	fflush(stdout);
	f_dist = fopen(ruta_db, "r");
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

	if ((fquery = fopen(ruta_queries, "r")) == NULL)
		printf("Error al abrir para lectura el archivo de queries: %s\n", ruta_queries);
	else
		printf("Abriendo  para lectura %s\n", ruta_queries);
	printf("\nCargando Consultas... ");
	fflush(stdout);
	for (i = 0; i < N_QUERIES;   i++) {
		if (leedato(datos, fquery) == ERROR || feof(fquery)) {
			printf("\n\nERROR :: N_QUERIES mal establecido, Menos queries que las indicadas\n\n");
			fflush(stdout);
			fclose(fquery);
			break;
		}
		copiavalor(Consultas[i], datos);
	}
	fclose(fquery);
	printf("OK\n");
	fflush(stdout);

	printf("\nLength of the arrays = %d" , LENGTH_ARRAY);
	printf("\nTOPK = %d" , TOPK);
	fflush(stdout);

	for( i = 0; i < dim; i++ ){
		for( j = 0; j < n_db; j++ )
		{
			CudaDB[j][i] = DB[i][j];
		}
	}

	//Allocating space to store the results
	if (cudaSuccess != cudaMalloc((void **)&res_final, sizeof(Elem)*TOPK*N_BLOQUES))
	{
		printf("\nERROR 1 :: cudaMalloc\n");
		cudaThreadExit();
		return 0;
	}
	res_final_H = (Elem *)malloc(sizeof(Elem)*TOPK*N_BLOQUES);
	for (i=0; i<TOPK*N_BLOQUES; i++)
	{
		res_final_H[i].ind = 0;
		res_final_H[i].dist = 0;
	}
	//Initializing res_final
	if (cudaSuccess != cudaMemset(res_final, 0, sizeof(Elem)*TOPK*N_BLOQUES))
	{
		printf("\nERROR 2 :: cudaMemset\n");
		cudaThreadExit();
		return 0;
	}

	 //Allocating space for the heaps in HEAPS_dev[TOPK][N_BLOQUES*512]. The elements of the heaps are accesed by columns to improve coalescing.
	if (cudaSuccess != cudaMallocPitch((void **)&HEAPS_dev, &pitch_H, sizeof(Elem)*N_BLOQUES*T_per_BLOCK, TOPK))
	{
		printf("\nERROR 3 :: cudaMallocPitch :: Heaps_dev col=%lld :: row=%d\n", (long long)(sizeof(Elem)*N_BLOQUES*T_per_BLOCK), TOPK);
		cudaThreadExit();
		return 0;
	}

	//Generating the arrays to be reduced
	arr_Dist_H = (Elem **)malloc(sizeof(Elem *)*N_BLOQUES);
	for (i=0; i<N_BLOQUES; i++)
		arr_Dist_H[i] = (Elem *)malloc(sizeof(Elem)*LENGTH_ARRAY);

	for (i=0; i<N_BLOQUES; i++)
		for (j=0; j<LENGTH_ARRAY; j++)
		{
			arr_Dist_H[i][j].ind = (LENGTH_ARRAY*i) + j; //Setting an ID
			arr_Dist_H[i][j].dist = (float)(0.1*(float)((LENGTH_ARRAY * i) + j));//Setting as distances the numbers 0, 0.1, 0.2, ...
		}

	 //Allocating space for the arrays to be reduced in arr_Dist[N_BLOQUES][LENGTH_ARRAY]. One array per row.
		if (cudaSuccess != cudaMallocPitch((void **)&arr_Dist, &pitch_Dist, LENGTH_ARRAY*sizeof(Elem), N_BLOQUES))
		{
			printf("\nERROR 4 :: cudaMallocPitch\n");
			cudaThreadExit();
			return 0;
		}

	//Copying the arrays to be reduced from host to device memory
		for (i=0; i < N_BLOQUES; i++)
			if (cudaSuccess != cudaMemcpy((char *)arr_Dist + (i*(int)pitch_Dist), (Elem *)(arr_Dist_H[i]), sizeof(Elem)*LENGTH_ARRAY, cudaMemcpyHostToDevice))
			{
				printf("\nERROR 5 :: cudaMemcpy\n");
				cudaThreadExit();
				return 0;
			}

	 //We finish with all the unresolved operations
			cudaThreadSynchronize();
			cudaDeviceSynchronize();
	//Measuring time
			getrusage(RUSAGE_SELF, &r1);
			gettimeofday(&t1, 0);

			printf("\nN_BLOQUES = %d :: T_per_BLOCK = %d\n", N_BLOQUES, T_per_BLOCK);
			fflush(stdout);

			Batch_Heap_Reduction<<< N_BLOQUES, T_per_BLOCK>>> (HEAPS_dev, (int)pitch_H, arr_Dist, (int)pitch_Dist, res_final);

			if (cudaSuccess != cudaMemcpy((Elem *)res_final_H, (Elem *)res_final, sizeof(Elem)*TOPK*N_BLOQUES, cudaMemcpyDeviceToHost))
			{
				printf("\nERROR 41 :: cudaMemcpy :: iteraH\n");
				cudaThreadExit();
				return 0;
			}

			cudaThreadSynchronize();
			cudaDeviceSynchronize();
			gettimeofday(&t2, 0);
			getrusage(RUSAGE_SELF, &r2);

			user_time = (r2.ru_utime.tv_sec - r1.ru_utime.tv_sec) + (r2.ru_utime.tv_usec - r1.ru_utime.tv_usec)/1000000.0;
			sys_time = (r2.ru_stime.tv_sec - r1.ru_stime.tv_sec) + (r2.ru_stime.tv_usec - r1.ru_stime.tv_usec)/1000000.0;
			real_time = (t2.tv_sec - t1.tv_sec) + (float)(t2.tv_usec - t1.tv_usec)/1000000;

			printf("\nK = %d", TOPK);
			printf("\nTiempo CPU = %f", user_time + sys_time);
			printf("\nTiempo Real = %f", real_time);
			fflush(stdout);

			for (i=0; i<N_BLOQUES; i++)
			{
				printf("\n\nResults array %d (smallest distances):", i);  
				for (j=TOPK*i; j<(TOPK*i)+TOPK; j++)
					printf("\nind = %d :: dist = %f", res_final_H[j].ind, res_final_H[j].dist);
			}
			printf("\n");

			cudaFree(HEAPS_dev);
			cudaFree(arr_Dist);

			cudaThreadExit();
			return 0;
		}

//Push an element 'elem' to the id-th heap stored in the id-th column of the matrix 'heap'
		__device__ void pushH(Elem *heap, Elem *elem, int *n_elem, int pitch, int id)
		{
			int i;
			Elem temp;

			((Elem *)((char *)heap + (*n_elem)*pitch))[id].dist = elem->dist;
			((Elem *)((char *)heap + (*n_elem)*pitch))[id].ind = elem->ind;
			(*n_elem)++;
			for (i = *n_elem; i>1 && ((Elem *)((char *)heap + (i-1)*pitch))[id].dist > ((Elem *)((char *)heap + ((i/2)-1)*pitch))[id].dist; i=i/2)
			{
		//Swap with the father
				temp.dist = ((Elem *)((char *)heap + (i-1)*pitch))[id].dist;
				temp.ind = ((Elem *)((char *)heap + (i-1)*pitch))[id].ind;
				((Elem *)((char *)heap + (i-1)*pitch))[id].dist = ((Elem *)((char *)heap + ((i/2)-1)*pitch))[id].dist;
				((Elem *)((char *)heap + (i-1)*pitch))[id].ind = ((Elem *)((char *)heap + ((i/2)-1)*pitch))[id].ind;
				((Elem *)((char *)heap + ((i/2)-1)*pitch))[id].dist = temp.dist;
				((Elem *)((char *)heap + ((i/2)-1)*pitch))[id].ind = temp.ind;
			}
			return;
		}

//Pop an element from id-th heap stored in the id-th column of the matrix 'heap' and stores it in 'eresult'
		__device__ void popH(Elem *heap, int *n_elem, int pitch, int id, Elem *eresult)
		{
			int i, k;
			Elem temp;
			eresult->dist = ((Elem *)((char *)heap+0))[id].dist;
			eresult->ind = ((Elem *)((char *)heap+0))[id].ind; 

	((Elem *)((char *)heap+0))[id].dist = ((Elem *)((char *)heap + ((*n_elem)-1)*pitch))[id].dist;//Moving the last element to the root
	((Elem *)((char *)heap+0))[id].ind = ((Elem *)((char *)heap + ((*n_elem)-1)*pitch))[id].ind;
	(*n_elem)--;
	i = 1;
	while(2*i <= *n_elem) //while exists some child
	{
		k = 2*i; //left child
		if(k+1 <= *n_elem && ((Elem *)((char *)heap + ((k+1)-1)*pitch))[id].dist > ((Elem *)((char *)heap + (k-1)*pitch))[id].dist)
			k = k+1;  //right child is the biggest

		if(((Elem *)((char *)heap + (i-1)*pitch))[id].dist > ((Elem *)((char *)heap + (k-1)*pitch))[id].dist)
			break;  //bigger than both childs

		temp.dist = ((Elem *)((char *)heap + (i-1)*pitch))[id].dist;
		temp.ind = ((Elem *)((char *)heap + (i-1)*pitch))[id].ind;
		((Elem *)((char *)heap + (i-1)*pitch))[id].dist = ((Elem *)((char *)heap + (k-1)*pitch))[id].dist;
		((Elem *)((char *)heap + (i-1)*pitch))[id].ind = ((Elem *)((char *)heap + (k-1)*pitch))[id].ind;
		((Elem *)((char *)heap + (k-1)*pitch))[id].dist = temp.dist;
		((Elem *)((char *)heap + (k-1)*pitch))[id].ind = temp.ind;
		i = k;   //swap with the biggest child
	}
	return;
}


//Returns the root of the id-th heap (stored in the id-th column)
__device__ float topH(Elem *heap, int id) //NOTE: Be careful if the heap is empty and topH is called, it will give an error
{
	return ((Elem *)((char *)heap + 0))[id].dist;
}

//Pop and push in one operation
__device__ void popushH(Elem *heap, Elem *elem, int *n_elem, int pitch, int id)
{
	int i, k;
	Elem temp;

	((Elem *)((char *)heap+0))[id].dist = elem->dist;
	((Elem *)((char *)heap+0))[id].ind  = elem->ind;

	i = 1;
	while(2*i <= *n_elem) //while exists some child
	{
		k = 2*i; //left child
		if(k+1 <= *n_elem && ((Elem *)((char *)heap + ((k+1)-1)*pitch))[id].dist > ((Elem *)((char *)heap + (k-1)*pitch))[id].dist)
			k = k+1;  //right child is the biggest

		if(((Elem *)((char *)heap + (i-1)*pitch))[id].dist > ((Elem *)((char *)heap + (k-1)*pitch))[id].dist)
			break;  //bigger than both childs

		temp.dist = ((Elem *)((char *)heap + (i-1)*pitch))[id].dist;
		temp.ind = ((Elem *)((char *)heap + (i-1)*pitch))[id].ind;
		((Elem *)((char *)heap + (i-1)*pitch))[id].dist = ((Elem *)((char *)heap + (k-1)*pitch))[id].dist;
		((Elem *)((char *)heap + (i-1)*pitch))[id].ind = ((Elem *)((char *)heap + (k-1)*pitch))[id].ind;
		((Elem *)((char *)heap + (k-1)*pitch))[id].dist = temp.dist;
		((Elem *)((char *)heap + (k-1)*pitch))[id].ind = temp.ind;
		i = k;   //swap with the bigger child
	}
	return;
}


__global__ void Batch_Heap_Reduction(Elem *heap, int pitch_H, Elem *arr_Dist, int pitch_Dist, Elem *res_final)
{
	int i, j, n_elem=0, n_elemWarp=0;
	int id;
	Elem eresult;
	__shared__ Elem matrizWarp[TOPK][TAM_WARP];
	__shared__ Elem heapfin[TOPK][1];

	id = threadIdx.x + (blockDim.x * blockIdx.x);

	//First Step: The array to be sorted (arr_Dist) is reduced to T_per_BLOCK heaps stored in device memory
	//The element of arr_Dist are distributed in a circular manner, therefore consecutive threads access to consecutive elements (consecutive memory addresses)
	for(i=threadIdx.x; i < LENGTH_ARRAY; i += blockDim.x)
	{
			if (n_elem >= TOPK)//If the current number of elements in the heap is >= than TOPK (really never it is > than TOPK, at most it is equal to TOPK)
			{
			//The next if is to add an element to the heap just if that element is less than the head of the heap
				if (topH(heap, id) > ((Elem *)((char *)arr_Dist + (blockIdx.x*pitch_Dist)))[i].dist)
						popushH(heap, &(((Elem *)((char *)arr_Dist + (blockIdx.x*pitch_Dist)))[i]), &n_elem, pitch_H, id); //Pop and Push in one operation
				}
				else
					pushH(heap, &(((Elem *)((char *)arr_Dist + (blockIdx.x*pitch_Dist)))[i]), &n_elem, pitch_H, id);
			}

			__syncthreads();


	//Second Step: the first warp of the CUDA Block reduces the elements of the heaps (stored in device memory) to TAM_WARP heaps stored in shared memory
			if (threadIdx.x < TAM_WARP)
			{
				for(j=id; j < blockDim.x*(blockIdx.x+1); j += TAM_WARP)
				{
					n_elem = TOPK;
					for(i=0; i < TOPK; i++)
					{
				 popH(heap, &n_elem, pitch_H, j, &eresult);//Getting an element from a heap in device memory

			//Adding the element to the heap in shared memory (if it corresponds)
				 if (n_elemWarp < TOPK)
				 	pushH(&(matrizWarp[0][0]), &eresult, &n_elemWarp, sizeof(Elem)*TAM_WARP, threadIdx.x);
				 else
				 	if (topH(&(matrizWarp[0][0]), threadIdx.x) > eresult.dist)
				 		popushH(&(matrizWarp[0][0]), &eresult, &n_elemWarp, sizeof(Elem)*TAM_WARP, threadIdx.x);
				 }
				}
			}


			__syncthreads();


	//Third Step: The first thread of the CUDA Block reduces the elements to one heap (stored in shared memory). The elements of this heap are the final results.
			if (threadIdx.x == 0)
			{
				n_elem = 0;
				for(j=0; j < TAM_WARP; j++)
				{
					for(i=0; i < TOPK; i++)
						if (n_elem < TOPK)
							pushH((Elem *)heapfin, &(matrizWarp[i][j]), &n_elem, sizeof(Elem), 0);
						else
							if (topH((Elem *)heapfin, 0) > matrizWarp[i][j].dist)
								popushH((Elem *)heapfin, &(matrizWarp[i][j]), &n_elem, sizeof(Elem), 0);
						}

		 //Writing the results
						for (i=TOPK*blockIdx.x; i<TOPK*(blockIdx.x+1); i++)
							popH(&(heapfin[0][0]), &n_elem, sizeof(Elem), 0, &(res_final[i]));
					}

					return;
				}

				void copiavalor(double *a, double *b) {
					int i;
					for (i = 0; i < DIM; i++)
						a[i] = b[i];
					return;
				}
				int leedato(double *dato, FILE *file) {
					int i = 0;

					for (i = 0; i < DIM; i++)
						if (fscanf(file, "%lf", &dato[i]) < 1)
							return ERROR;
						return 1;
					}