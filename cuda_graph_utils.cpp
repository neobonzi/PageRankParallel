#include "GraphUtils.h"

namespace GraphUtils
{
NodeMatrix::NodeMatrix(int w) : width(w) 
{
    width = w;
    matrix = (double *)malloc(w * w * sizeof(double));
    for (int i = 0; i < w*w; i++) {
        matrix[i] = 0;
    }
}

void NodeMatrix::print() {
    for (int r = 0; r < width; r++) {
        for (int c = 0; c < width; c++) {
            cout << matrix[INDEX(r, c, width)] << " ";
        }
        cout << endl;
    }
}

// Converts a NodeGraph stored as an adjacency list to NodeMatrix stored as an
// adjacency matrix.
NodeMatrix *listToMatrix(NodeGraph *node_graph) {
   NodeMatrix *node_matrix = new NodeMatrix(node_graph->size());
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
         if (node_A->outDegree != 0) {
            node_matrix->matrix
               [INDEX(node_B->id_num /* row */,
                      node_A->id_num /* col */,
                      node_matrix->width)] += 1/(double)(node_B->outDegree);
         }
      }
   }
   return node_matrix;       
}

// Must free the data after you're done using it.
double* matrixToPrestige(NodeMatrix *node_matrix) {
   const int width = node_matrix->width;
   double init_val = 1/(double)width;
   double *prestige = (double *) malloc(width*sizeof(double));

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
        
        for(map<string, Node *>::iterator refIter = node->referencedBy.begin(); refIter != node->referencedBy.end(); ++refIter)
        {
            cout << "\t" << refIter->first << " "
                 << refIter->second->id_num << endl;
        }
    }
}
}
