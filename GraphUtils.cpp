#include "GraphUtils.h"

namespace GraphUtils {
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
   float *prestige = (float *) malloc(width*sizeof(float));

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
        cout << "Node: " << iter->first << " ID: " << node->id_num;
        cout << " [Out " << node->outDegree << " In " << node->inDegree << "]" << endl;
        
        for(map<string, Node *>::iterator refIter = node->referencedBy.begin(); refIter != node->referencedBy.end(); ++refIter)
        {
            cout << "\t" << refIter->first << " "
                 << refIter->second->id_num << endl;
        }
    }
}
} // namespace GraphUtils
