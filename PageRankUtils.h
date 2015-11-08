#ifndef PAGERANK_UTILS_H_
#define PAGERANK_UTILS_H_
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>
#include "GraphUtils.h"

#include "Node.h"

using namespace std;

namespace PageRankUtils
{

/**
 * Computes the prestige for a node given an index into
 * a prestige array, the prestige array and a weighted adjacency matrix
 */
void updatePrestige(int nodeIndex, vector<Node *> prestiges, GraphUtils::NodeMatrix *nodeMatrix)
{
    double prestige = 0;
    Node *node = prestiges[nodeIndex];

    for (int row = 0; row < nodeMatrix->width; row++)
    {
        prestige += prestiges[row]->curRank * nodeMatrix->matrix[INDEX(row, nodeIndex, nodeMatrix->width)];
    }

    node->updatePrestige(prestige);
}
}
#endif
