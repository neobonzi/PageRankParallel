#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_
#include <cstdlib>
#include <iostream>
#include <iterator>

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
      vector<float> matrix;
      vector<Node *> nodes;
      int width;

      NodeMatrix(int w, float initial_value) : width(w), matrix(w*w, initial_value) {}

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
NodeMatrix *listToMatrix(NodeGraph *node_graph) {
   NodeMatrix *node_matrix = new NodeMatrix(node_graph->size(),
                                            1/(float)node_graph->size());
   for (NodeGraph::iterator graphIt = node_graph->begin();
        graphIt!= node_graph->end();
        ++graphIt)
   {
      Node *node_A = graphIt->second;
      node_matrix->nodes.push_back(node_A);
      for(map<string, Node *>::iterator refIt = node_A->referencedBy.begin();
          refIt != node_A->referencedBy.end();
          ++refIt)
      {
         Node *node_B = refIt->second;
         // node_A points to node_B
         node_matrix->matrix
            [INDEX(node_A->id_num /* row */,
                   node_B->id_num /* col */,
                   node_matrix->width)] += 1/(float)node_A->outDegree;
      }
   }
   return node_matrix;       
}

// Must free the data after you're done using it.
float* matrixToPrestige(NodeMatrix *node_matrix) {
   const int width = node_matrix->width;
   float init_val = 1/(float)width;
   float *prestige = (float *) malloc(width*width*sizeof(float));

   if (prestige == NULL) {
      perror("malloc");
      exit(-1);
   }

   for (int i = 0 ; i < width; i++) {
      prestige[i] = init_val;
   }

   return prestige;
}

void printNodeGraph(NodeGraph *graph)
{
    //Spit out the keys and values in map
    for(NodeGraph::iterator iter = graph->begin(); iter != graph->end(); ++iter)
    {
        string k = iter->first;
        Node *node = iter->second;
        cout << "Node: " << iter->first << " " << node->id_num;
        cout << " Out " << node->outDegree << " In " << node->inDegree << endl;
        
        for(map<string, Node *>::iterator refIter = node->referencedBy.begin(); refIter != node->referencedBy.end(); ++refIter)
        {
            cout << "\t" << refIter->first << " "
                 << refIter->second->id_num << endl;
        }
    }
}
}
#endif
