#include "libreria.h"

#define DIM DEFINE_DIMENSION

#define T_x_BLOCK devProp_maximumThreadsPerBlock 

#define ERROR -1

#define TOPK DEFINE_TOPK

#define NE DEFINE_N_ELEM

#define TAM_WARP 32 //Num de threads maximo de un warp

#define Q 3999 

struct _Elem
{
  double dist;
  int ind;
};
typedef struct _Elem Elem;

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
    }