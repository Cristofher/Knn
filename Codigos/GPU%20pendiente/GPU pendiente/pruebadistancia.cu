#include <stdio.h>
#define N 10
#define M 20

__global__ void matrix_add(int *matrix_A, size_t pitch_A, int *matrix_B, size_t pitch_B, int *matrix_C, size_t pitch_C)
{
	int row, col;
	row = threadIdx.x;
	col = threadIdx.y;
	for (int i = 0; i < row; ++i){
		for (int j = 0; j < col; ++j){
			matrix_C[i][j]=matrix_A[i][j]+matrix_B[i][j];
		}	
	}
	
}


int main()
{
	int a[N][M], b[N][M], c[N][M], i, j;
	int *matrix_C, *matrix_B, *matrix_A;
	size_t pitch_A, pitch_B, pitch_C;


	/* allocate space for device copies of a, b, c */
	/* the pitch_A, pitch_B and pitch_C are the assigned sizes of a row in the matrices matrix_A, matrix_B and matrix_C respectively */
	if (cudaSuccess != cudaMallocPitch((void **)&matrix_A, &pitch_A, N * sizeof(int), M))
	{
		printf("\nERROR :: cudaMallocPitch :: matrix_A\n");
		cudaThreadExit();
		return 0;
	}

	if (cudaSuccess != cudaMallocPitch((void **)&matrix_B, &pitch_B, N * sizeof(int), M))
	{
		printf("\nERROR :: cudaMallocPitch :: matrix_B\n");
		cudaThreadExit();
		return 0;
	}

	if (cudaSuccess != cudaMallocPitch((void **)&matrix_C, &pitch_C, N * sizeof(int), M))
	{
		printf("\nERROR :: cudaMallocPitch :: matrix_C\n");
		cudaThreadExit();
		return 0;
	}

	for( i = 0; i < N; i++ )
		for( j = 0; j < M; j++ ){
			a[i][j] = 1;
			b[i][j] = 10;
			c[i][j] = 0;
		}

	printf( "\nMatriz A:\n\n");
	for( i = 0; i < N; i++ ){
		for( j = 0; j < M; j++ )
			printf( "%d ", a[i][j] );
		printf( "\n");
	}

	printf( "\nMatriz B:\n\n");
	for( i = 0; i < N; i++ ){
		for( j = 0; j < M; j++ )
			printf( "%d ", b[i][j] );
		printf( "\n");
	}

	//Copying from host to device the 2D-matrices a and b
	cudaMemcpy2D(matrix_A, pitch_A, *a, sizeof(int)*N, sizeof(int)*N, M, cudaMemcpyHostToDevice);
	cudaMemcpy2D(matrix_B, pitch_B, *b, sizeof(int)*N, sizeof(int)*N, M, cudaMemcpyHostToDevice);

	// Allocate CUDA events that we'll use for timing
	cudaEvent_t start;
	cudaEvent_t stop;

	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	// Record the start event
	cudaEventRecord(start, NULL);


	/* launch the kernel on the GPU with 1 CUDA Block and N threads per CUDA Block*/
	matrix_add<<< 1, M >>>( matrix_A, pitch_A, matrix_B, pitch_B, matrix_C, pitch_C);


	// Record the stop event
	cudaEventRecord(stop, NULL);

	// Wait for the stop event to complete
	cudaEventSynchronize(stop);

	float msecTotal = 0.0f;
	cudaEventElapsedTime(&msecTotal, start, stop);

	printf("Running Time (msecs.) = %f\n", msecTotal);

	/* copy result back to host */
	//Copying from host to device the 2D-matrices a and b
	cudaMemcpy2D(*c, sizeof(int)*N, matrix_C, pitch_C, sizeof(int)*N, M, cudaMemcpyDeviceToHost);

	printf( "\nMatriz C:\n\n");
	for( i = 0; i < N; i++ ){
		for( j = 0; j < M; j++ )
			printf( "%d ", c[i][j] );
		printf( "\n");
	}
	
	/* clean up */
	cudaFree( matrix_A );
	cudaFree( matrix_B );
	cudaFree( matrix_C );
	
	return 0;
} /* end main */



