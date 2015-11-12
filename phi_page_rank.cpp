#include <stdio.h>
#include "GraphUtils.h"
#include "PageRank.h"
#include "mkl.h"
#include <string.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>

#define RANDOM_WEIGHT 0.95
#define DELTA 0.0001
using namespace GraphUtils;

void addRandomness(double *prestiges, int count)
{
    #pragma simd
    for(int i = 0; i < count; i++)
    {
       prestiges[i] *= RANDOM_WEIGHT;
       prestiges[i] += ((1.0 - RANDOM_WEIGHT) * (1.0 / ((double)count)));
    }
}

int checkConvergence(double *oldPrestiges, double *newPrestiges, int count, double delta)
{
    double sum = 0.0;

    for(int i = 0; i < count; i++)
    {
        //cout << "old " << oldPrestiges[i] << " new " << newPrestiges[i] << endl;
        double difference = oldPrestiges[i] - newPrestiges[i];
        sum += abs(difference); 
    }

    if (sum > DELTA)
    {
        cout << "NOPE: " << sum;
        return 0;
    }

    cout << "converged: " << sum;
    return 1;
}

void matrixMultiply(double *prestige, double *matrix, double *result, int width)
{
    for (int x = 0; x < width; x++)
    {
        double sum = 0.0;
        for(int y = 0; y < width; y++)
        {
            //cout << prestige[y] << " * " << matrix[(x * width) + y] << " =  " ;
            sum = sum + (prestige[y] * matrix[(x * width) + y]);
            //cout << sum << " + " << endl;
        }
        //cout << " = " << sum << endl;
        result[x] = sum;
        //cout << "Sum: " << result[x];
    }
}

void pageRank(NodeGraph *graph)
{
    NodeMatrix *matrix = listToMatrix(graph);
    const int width = matrix->width;
    double *prestige = GraphUtils::matrixToPrestige(matrix);
    
    const int iterations = 10000;

    //SGEMM Constants
    double alpha, beta;
    alpha = 1.0;
    beta = 0.0;
    double *newPrestiges = (double *)mkl_malloc(sizeof(double) * width, 64);

    int counter = 0;

    cout << "Printing matrix" << endl;
    //matrix->print();
    while(counter < iterations)
    {
        matrixMultiply(prestige, matrix->matrix, newPrestiges, width);
        //cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
            //1, width, width, alpha, prestige, width, matrix->matrix, width,
            //beta, newPrestiges, width);

        //cout << "new Prestiges: ";
        //for(int z = 0; z < width; z++)
        //{
        //    cout << newPrestiges[z] << " ";
        //}
        cout << endl;

        addRandomness(newPrestiges, width);
        if (counter != 0 && checkConvergence(prestige, newPrestiges, width, DELTA) == 1)
        {
            fprintf(stderr, "Converged after %d iterations\n", counter);
            memcpy(prestige, newPrestiges, width);
            break;
        }
        memcpy(prestige, newPrestiges, width * sizeof(double));

        counter++;
    }
    for(int k = 0; k < width; k++)
    {
        fprintf(stderr, "%d: %lf, ",k, prestige[k]);
    }
    fprintf(stderr, "\n");

}
