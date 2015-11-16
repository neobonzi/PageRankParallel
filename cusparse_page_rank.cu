#include <algorithm>
#include <cuda_runtime.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <cusparse.h>

#include "GraphUtils.h"
#include "cusparse_graph_utils.h"
#include "Node.h"
#include "CSVReader.h"
#include "SNAPReader.h"

#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef ITERATIONS
#define ITERATIONS 1000 
#endif

#ifndef D
#define D 0.95
#endif

#ifndef max
#define max(a,b) ((a > b) ? a : b)
#endif
#define CSV_DELIMITER ","

using namespace std;
using namespace CSVReader;
using namespace SNAPReader;

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

static void check_cusparse_err(cusparseStatus_t err, const char* file,
                               int line) {
  if (err != CUSPARSE_STATUS_SUCCESS) {
    printf( "%s in %s at line %d\n", ( err ),
        file, line );
    exit( EXIT_FAILURE );
  }
}

#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))
#define HANDLE_CUSPARSE( err ) (check_cusparse_err( err, __FILE__, __LINE__ ))

void PageRankKernel(double *val, int *rowptr, int *colndx,
                    double *p, double *pr, int width, int nnz, double *ones) {
   cusparseHandle_t handle;
   HANDLE_CUSPARSE(cusparseCreate(&handle));
   cusparseMatDescr_t descr;
   HANDLE_CUSPARSE(cusparseCreateMatDescr(&descr));
   double multiplier = D;
   double constant = (1-D)/(double)width;

   HANDLE_CUSPARSE(cusparseDcsrmv(handle, CUSPARSE_OPERATION_NON_TRANSPOSE,
                  width, width, nnz, &multiplier,
                  descr,
                  val,
                  rowptr, colndx,
                  p, &constant,
                  pr)); 
   cudaDeviceSynchronize();
}

void PageRankOnDevice(CSRMatrix *matrix, double *prestige, double *ones) {
   double *dVal, *dPrestige, *dPrestigeResult, *dOnes;
   int *dRowPtr, *dColInd;
   const int w = matrix->width;
   const int nnz = matrix->nnz;
   // Allocate memory.
   // CSR format
   HANDLE_ERROR(cudaMalloc(&dVal, nnz*sizeof(double)));
   HANDLE_ERROR(cudaMemcpy(dVal, matrix->csrValA, nnz*sizeof(double),
                cudaMemcpyHostToDevice));
   HANDLE_ERROR(cudaMalloc(&dColInd, nnz*sizeof(int)));
   HANDLE_ERROR(cudaMemcpy(dColInd, matrix->csrColIndA, nnz*sizeof(int),
                cudaMemcpyHostToDevice));
   HANDLE_ERROR(cudaMalloc(&dRowPtr, (w+1)*sizeof(int)));
   HANDLE_ERROR(cudaMemcpy(dRowPtr, matrix->csrRowPtrA, (w+1)*sizeof(int),
                cudaMemcpyHostToDevice));
   // Prestige
   HANDLE_ERROR(cudaMalloc(&dPrestige, w*sizeof(double)));
   HANDLE_ERROR(cudaMemcpy(dPrestige, prestige, w*sizeof(double),
                cudaMemcpyHostToDevice));
   HANDLE_ERROR(cudaMalloc(&dOnes, w*sizeof(double)));
   HANDLE_ERROR(cudaMemcpy(dOnes, ones, w*sizeof(double),
                cudaMemcpyHostToDevice));
   // Prestige Result
   HANDLE_ERROR(cudaMalloc(&dPrestigeResult, w*sizeof(double)));

   // Launch kernel for page rank
   for (int i = 0; i < ITERATIONS; i+=2) {
      HANDLE_ERROR(cudaMemcpy(dPrestigeResult, dOnes, w*sizeof(double),
                cudaMemcpyDeviceToDevice));
      PageRankKernel(dVal, dRowPtr, dColInd, dPrestige, dPrestigeResult, w,
                     nnz, dOnes);
      HANDLE_ERROR(cudaMemcpy(dPrestige, dOnes, w*sizeof(double),
                   cudaMemcpyDeviceToDevice));
      PageRankKernel(dVal, dRowPtr, dColInd, dPrestigeResult, dPrestige, w,
                     nnz, dOnes);
   }

   // Copy result to host.
   HANDLE_ERROR(cudaMemcpy(prestige, dPrestige, w*sizeof(double),
                cudaMemcpyDeviceToHost));

   // Free memory.
   HANDLE_ERROR(cudaFree(dOnes));
   HANDLE_ERROR(cudaFree(dVal));
   HANDLE_ERROR(cudaFree(dRowPtr));
   HANDLE_ERROR(cudaFree(dColInd));
   HANDLE_ERROR(cudaFree(dPrestige));
   HANDLE_ERROR(cudaFree(dPrestigeResult));
}

void updateNodePrestige(vector<Node *> nodes, double *prestige) {
   for (int i = 0; i < nodes.size(); i++) {
      nodes[i]->updatePrestige(prestige[i]);
   }
}

void pageRank(GraphUtils::NodeGraph *graph) {
   CSRMatrix *matrix = listToCSRMatrix(graph);
   const int width = matrix->width;
   double *prestige = createPrestigeVector(width);
   double *ones = (double *) malloc(width*sizeof(double));
   for (int i = 0; i < width; i++)
      ones[i] = 1;

   PageRankOnDevice(matrix, prestige, ones);

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

int main(int argc, char **argv)
{
   string fileName = string(argv[1]);
   GraphUtils::NodeGraph *graph;

   //Check the filename to see if its SNAP or CSV
   if(fileName.substr(fileName.find_last_of(".") + 1) == "csv") {
      graph = readCSVData(argv[1]);
   }
   else {
      graph = readSNAPData(argv[1]);
   }

   pageRank(graph);
   return 0;
}
