#include <stdio.h>
#define N 10

__global__ void matrix_add(double *matrix_A, size_t pitch_A, double *matrix_B, size_t pitch_B, double *matrix_C)
{
	int col, row;
	double d = 0, resultado=0;
	__shared__ double query[N];
	
	if (threadIdx.x < N)
		query[threadIdx.x] = matrix_A[threadIdx.x][blockIdx.x];
	__syncthreads();

	//row = threadIdx.x + blockDim.x * blockIdx.x;
	
	for (col=threadIdx.x; col < N; col += blockDim.x)
	{
		d=0;
		for (row=0; row < N; row++){
			resultado = query[row] - ((double *)((char *)matrix_B + row*(int)pitch_B))[col];
			d += (resultado * resultado);
			//matrix_C[row] = ((row1-row2)*(row1-row2));
			//printf("matrix_C[%d] ==> %lf\n",row,matrix_C[col]);
		}
		matrix_C[col] = sqrt(d);
	}
}
	
__global__ void square(double *matrix_C)
{
	matrix_C[threadIdx.x] = sqrt((double)threadIdx.x);
}


int main()
{
	double a[N][N], b[N][N];
	int i, j;
	double c[N], *matrix_C;
	double  *matrix_B, *matrix_A;
	size_t pitch_A, pitch_B;
	double size = N * sizeof( double );

	if (cudaSuccess != cudaMallocPitch((void **)&matrix_A, &pitch_A, N * sizeof(double), N)){
		printf("\nERROR :: cudaMallocPitch :: matrix_A\n");
		cudaThreadExit();
		return 0;
	}

	if (cudaSuccess != cudaMallocPitch((void **)&matrix_B, &pitch_B, N * sizeof(double), N)){
		printf("\nERROR :: cudaMallocPitch :: matrix_B\n");
		cudaThreadExit();
		return 0;
	}

	cudaMalloc( (void **) &matrix_C, size );

	for( i = 0; i < N; i++ )
		for( j = 0; j < N; j++ ){
			a[i][j] = j+(i*N);
			b[i][j] = 5*(j+(i*N));
			c[j] = 0;
		}

		printf( "\nMatriz A:\n\n");
		for( i = 0; i < N; i++ ){
			for( j = 0; j < N; j++ )
				printf( "%lf ", a[i][j] );
			printf( "\n");
		}

		printf( "\nMatriz B:\n\n");
		for( i = 0; i < N; i++ ){
			for( j = 0; j < N; j++ )
				printf( "%lf ", b[i][j] );
			printf( "\n");
		}

	//Copying from host to device the 2D-matrices a and b
		cudaMemcpy2D(matrix_A, pitch_A, *a, sizeof(double)*N, sizeof(double)*N, N, cudaMemcpyHostToDevice);
		cudaMemcpy2D(matrix_B, pitch_B, *b, sizeof(double)*N, sizeof(double)*N, N, cudaMemcpyHostToDevice);

	// Allocate CUDA events that we'll use for timing
		cudaEvent_t start;
		cudaEvent_t stop;

		cudaEventCreate(&start);
		cudaEventCreate(&stop);

	// Record the start event
		cudaEventRecord(start, NULL);


	/* launch the kernel on the GPU with 1 CUDA Block and N threads per CUDA Block*/
		matrix_add<<< 1, N >>>( matrix_A, pitch_A, matrix_B, pitch_B, matrix_C);

	// Record the stop event
		cudaEventRecord(stop, NULL);

	// Wait for the stop event to complete
		cudaEventSynchronize(stop);

		float msecTotal = 0.0f;
		cudaEventElapsedTime(&msecTotal, start, stop);

		printf("Running Time (msecs.) = %f\n", msecTotal);

	/* copy result back to host */
	//Copying from host to device the 2D-matrices a and b
		cudaMemcpy(c, matrix_C, size, cudaMemcpyDeviceToHost);

		printf( "\nMatriz C:\n\n");
		for( j = 0; j < N; j++ )
			printf( "%lf ", c[j] );
		printf( "\n");

	/* clean up */
		cudaFree( matrix_A );
		cudaFree( matrix_B );
		cudaFree( matrix_C );

		return 0;
} /* end main */



