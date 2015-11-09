#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <cstdio>
#include "Node.h"

using namespace std;

// INDEX specifies how the NodeMatrix will be indexed.
// It can be defined as either "col_major" or "row_major".
#ifndef INDEX
#define INDEX col_major
#endif

namespace GraphUtils
{

typedef map<string, Node *> NodeGraph;

// returns an index for column major format,
// assuming a square matrix of size (size).
#define col_major(row, col, size) (col)*(size)+(row)
// returns an index for row major format,
// assuming a square matrix of size (size).
#define row_major(row, col, size) (row)*(size)+(col)

//Ex: GraphUtils::NodeMatrix *matrix = GraphUtils::listToMatrix(graph);
class NodeMatrix {
   public:
      float *matrix;
      vector<Node *> nodes;
      int width;

      NodeMatrix(int w, float initial_value) : width(w) 
      {
          matrix = (float *)malloc(w * w * sizeof(float));
          for (int i = 0; i < w*w; i++) {
             matrix[i] = initial_value; 
          }
      }

      void print() {
         for (int r = 0; r < width; r++) {
            for (int c = 0; c < width; c++) {
               cout << matrix[INDEX(r, c, width)] << " ";
            }
            cout << endl;
         }
      }
};

// Converts a NodeGraph stored as an adjacency list to NodeMatrix stored as an
// adjacency matrix.
NodeMatrix *listToMatrix(NodeGraph *node_graph);
// Must free the data after you're done using it.
float* matrixToPrestige(NodeMatrix *node_matrix);
void printNodeGraph(NodeGraph *graph);
}  // namespace GraphUtils
#endif
