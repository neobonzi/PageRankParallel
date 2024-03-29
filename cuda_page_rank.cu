#include "PageRank.h"
#include <algorithm>
#include <cuda_runtime.h>

#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define TILEWIDTH 32

#ifndef ITERATIONS
#define ITERATIONS 300 
#endif

#ifndef D
#define D 0.95
#endif

#ifndef max
#define max(a,b) ((a > b) ? a : b)
#endif

// Handles an error for cuda calls.
static void HandleError( cudaError_t err,
    const char *file,
    int line ) {
  if (err != cudaSuccess) {
    printf( "%s in %s at line %d\n", cudaGetErrorString( err ),
        file, line );
    exit( EXIT_FAILURE );
  }
}

#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

// Exits gracefully if malloc returns a NULL pointer.
void *check_malloc(size_t size) {
   void *ptr = malloc(size);
   if (ptr == NULL) {
      fprintf(stderr, "malloc failed.\n");
      exit(-1);
   }
   return ptr;
}

__global__ void PageRankKernel(double *Md, int rows_M, int cols_M,
                               double *Nd, int rows_N, int cols_N,
                               double *Pd, int rows_P, int cols_P,
                               int tiles) {
   __shared__ double Mds[TILEWIDTH][TILEWIDTH];
   __shared__ double Nds[TILEWIDTH][TILEWIDTH];
   int row = blockIdx.y*TILEWIDTH + threadIdx.y;
   int col = blockIdx.x*TILEWIDTH + threadIdx.x;
   double pVal = 0; 
   int m, k;
   for (m = 0; m < tiles; m++) {
      Mds[threadIdx.y][threadIdx.x] = 
         (row >= rows_M) || ((m*TILEWIDTH + threadIdx.x) >= cols_M) ?
            0 : Md[row*cols_M + (m*TILEWIDTH + threadIdx.x)];
      
      Nds[threadIdx.y][threadIdx.x] =
         (col >= cols_N) || ((m*TILEWIDTH + threadIdx.y) >= rows_N) ?
         0 : Nd[col + (m*TILEWIDTH + threadIdx.y)*cols_N];
      __syncthreads();
      for (k = 0; k < TILEWIDTH; k++) {
         pVal += Mds[threadIdx.y][k]*Nds[k][threadIdx.x];
      }
      __syncthreads();
   }
   if (row < rows_P && col < cols_P) {
      Pd[row*cols_P + col] = D*pVal + (1-D)/(double)rows_P;
   }
}

void PageRankOnDevice(double *matrix, int rows_matrix, int cols_matrix,
                      double *N, int rows_N, int cols_N) {
   double *matrixd, *Nd, *Pd;
   // Allocate memory.
   int size_matrix = rows_matrix*cols_matrix*sizeof(double);
   HANDLE_ERROR(cudaMalloc(&matrixd, size_matrix));
   HANDLE_ERROR(cudaMemcpy(matrixd, matrix, size_matrix, cudaMemcpyHostToDevice));
   int size_N = rows_N*cols_N*sizeof(double);
   HANDLE_ERROR(cudaMalloc(&Nd, size_N));
   HANDLE_ERROR(cudaMemcpy(Nd, N, size_N, cudaMemcpyHostToDevice));
   HANDLE_ERROR(cudaMalloc(&Pd, size_N));

   // Launch kernel for the matrix multiply.
   int result_width = ceil(cols_N/(double)TILEWIDTH);
   int result_len = ceil(rows_N/(double)TILEWIDTH);
   dim3 dimGrid(result_width, result_len);
   dim3 dimBlock(TILEWIDTH, TILEWIDTH);
   int tiles = max(max(rows_matrix, cols_matrix), max(rows_N, cols_N));
   for (int i = 0; i < ITERATIONS; i+=2) {
      PageRankKernel<<<dimGrid, dimBlock>>>(matrixd, rows_matrix, cols_matrix,
                                            Nd, rows_N, cols_N,
                                            Pd, rows_N, cols_N,
                                            tiles);
      PageRankKernel<<<dimGrid, dimBlock>>>(matrixd, rows_matrix, cols_matrix,
                                            Pd, rows_N, cols_N,
                                            Nd, rows_N, cols_N,
                                            tiles);
   }

   // Copy result to host.
   HANDLE_ERROR(cudaMemcpy(N, Nd, size_N, cudaMemcpyDeviceToHost));

   // Free memory.
   HANDLE_ERROR(cudaFree(matrixd));
   HANDLE_ERROR(cudaFree(Nd));
   HANDLE_ERROR(cudaFree(Pd));
}

void updateNodePrestige(vector<Node *> nodes, double *prestige) {
   for (int i = 0; i < nodes.size(); i++) {
      nodes[i]->updatePrestige(prestige[i]);
   }
}

void pageRank(GraphUtils::NodeGraph *graph) {
   GraphUtils::NodeMatrix *matrix = GraphUtils::listToMatrix(graph);
   const int width = matrix->width;
   double *prestige = GraphUtils::matrixToPrestige(matrix);

   PageRankOnDevice(matrix->matrix, width, width,
                    prestige, width, 1);

   // update Node objects in vertex
   updateNodePrestige(matrix->nodes, prestige);
   free(prestige);

   // sort result and print out ranking
   std::sort(matrix->nodes.begin(), matrix->nodes.end(), Node::CompareByRank());
   for (int i = 0; i < matrix->nodes.size(); i++) {
      Node *node = matrix->nodes[i];
      printf("%d: %s with rank %f\n",
             i + 1, node->identifier.c_str(), node->curRank); 
   }
}
