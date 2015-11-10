#include <stdio.h>
#include "GraphUtils.h"
#include "PageRank.h"
#include "mkl.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define RANDOM_WEIGHT .95f
#define DELTA .0001f
using namespace GraphUtils;

void addRandomness(double *prestiges, int count)
{
    #pragma simd
    for(int i = 0; i < count; i++)
    {
       prestiges[i] *= RANDOM_WEIGHT;
       prestiges[i] += ((1.0f - RANDOM_WEIGHT) * (1.0f / ((double)count)));
    }
}

int checkConvergence(double *oldPrestiges, double *newPrestiges, int count, double delta)
{
    if (*newPrestiges == -1)
        return 0;

    long sum = 0.0f;
    #pragma simd
    for(int i = 0; i < count; i++)
    {
        sum += abs((long)(oldPrestiges[i] - newPrestiges[i])); 
    }

    if (sum > DELTA)
    {
        cout << "NOPE: " << sum;
        return 0;
    }

    cout << "converged: " << sum;
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

    int counter = 0;

    while(1)
    {
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
            1, width, width, alpha, prestige, width, matrix->matrix, width,
            beta, newPrestiges, width);
 
        addRandomness(newPrestiges, width);

        if (counter != 0 && checkConvergence(prestige, newPrestiges, width, DELTA) == 1)
        {
            fprintf(stderr, "Converged after %d iterations\n", counter);
            memcpy(prestige, newPrestiges, width);
            break;
        }
        memcpy(prestige, newPrestiges, width);

        counter++;
    }
    for(int k = 0; k < width; k++)
    {
        fprintf(stderr, "%d: %lf, ",k, prestige[k]);
    }
    fprintf(stderr, "\n");

}
