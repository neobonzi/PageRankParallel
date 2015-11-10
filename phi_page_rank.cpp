#include <stdio.h>
#include "GraphUtils.h"
#include "PageRank.h"
#include "mkl.h"
#include <string.h>
#include <stdlib.h>

using namespace GraphUtils;

void pageRank(NodeGraph *graph)
{
    NodeMatrix *matrix = listToMatrix(graph);
    const int width = matrix->width;
    double *prestige = GraphUtils::matrixToPrestige(matrix);

    const int iterations = 6;

    //SGEMM Constants
    double alpha, beta;
    alpha = 1.0;
    beta = 0.0;
    double *newPrestiges = (double *)mkl_malloc(sizeof(double) * width, 64);

    for (int i = 0; i < iterations; i++)
    {
        fprintf(stderr, "Current prestige: ");
        for(int k = 0; k < width; k++)
        {
            fprintf(stderr, "%lf", prestige[k]);
        }
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                1, width, width, alpha, prestige, width, matrix->matrix, width,
                beta, newPrestiges, width);

        memcpy(prestige, newPrestiges, width);
    }
}
