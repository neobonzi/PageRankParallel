#include <stdio.h>
#include "GraphUtils.h"
#include "PageRank.h"
#include "mkl.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define RANDOM_WEIGHT .85
#define DELTA .00000000005
using namespace GraphUtils;

void addRandomness(double *prestiges, int count)
{
    #pragma simd
    for(int i = 0; i < count; i++)
    {
       prestiges[i] *= RANDOM_WEIGHT;
       prestiges[i] += ((1.0 - RANDOM_WEIGHT) * (1 / ((double)count)));
    }
}

int checkConvergence(double *oldPrestiges, double *newPrestiges, int count, double delta)
{
    if (*newPrestiges == -1)
        return 0;

    for(int i = 0; i < count; i++)
    {
        if(abs((long)(oldPrestiges[i] - newPrestiges[i])) > DELTA)
            return 0;
    }

    return 1;
}

void pageRank(NodeGraph *graph)
{
    NodeMatrix *matrix = listToMatrix(graph);
    const int width = matrix->width;
    double *prestige = GraphUtils::matrixToPrestige(matrix);
    
    const int iterations = 100;

    //SGEMM Constants
    double alpha, beta;
    alpha = 1.0;
    beta = 0.0;
    double *newPrestiges = (double *)mkl_malloc(sizeof(double) * width, 64);
    *newPrestiges = -1;

    int counter = 0;
    while(checkConvergence(prestige, newPrestiges, width, DELTA) == 0)  
    {
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
            1, width, width, alpha, prestige, width, matrix->matrix, width,
            beta, newPrestiges, width);

        memcpy(prestige, newPrestiges, width);
        addRandomness(prestige, width);

        counter++;
    }
    fprintf(stderr, "Converged after %d iterations\n", counter);
     for(int k = 0; k < width; k++)
    {
        fprintf(stderr, "%lf", prestige[k]);
    }
    fprintf(stderr, "\n");

}
