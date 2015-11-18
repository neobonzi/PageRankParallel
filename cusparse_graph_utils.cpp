#include "cusparse_graph_utils.h"

using namespace GraphUtils;

// Exits gracefully if malloc returns a NULL pointer.
void *check_malloc(size_t size) {
   void *ptr = malloc(size);
   if (ptr == NULL) {
      perror("malloc");
      exit(-1);
   }
   return ptr;
}

// Initializes CSRMatrix with 0 based indexing.
CSRMatrix::CSRMatrix(int w) : width(w) {
   csrRowPtrA = (int *) check_malloc((w+1)*sizeof(int));
   csrRowPtrA[0] = 0;
   csrRowPtrA[w] = w;
   nnz = 0;
}

void CSRMatrix::setNnz(int val) {
   nnz = val;
   csrValA = (double *) malloc(nnz*sizeof(double));
   csrColIndA = (int *) malloc(nnz*sizeof(int));
}

CSRMatrix *listToCSRMatrix(GraphUtils::NodeGraph *node_graph) {
   CSRMatrix *csr_matrix = new CSRMatrix(node_graph->size());
   int nnz = 0;

   // Fill in nodes vector and count number of non zero values.
   for (NodeGraph::iterator graphIt = node_graph->begin();
        graphIt!= node_graph->end();
        ++graphIt) {
      Node *node_A = graphIt->second;
      csr_matrix->nodes.push_back(node_A);
      for(map<string, Node *>::iterator refIt = node_A->referencedBy.begin();
          refIt != node_A->referencedBy.end();
          ++refIt) {
         Node *node_B = refIt->second;
         if (node_B->outDegree != 0) {
            nnz++;
         }
      }
   }

   // Set nnz value and allocate val and col matrices
   csr_matrix->setNnz(nnz);

   int last_row_seen = 1, cur_index = 0;
   // Fill in values;
   for (NodeGraph::iterator graphIt = node_graph->begin();
        graphIt!= node_graph->end();
        ++graphIt) {
      Node *node_A = graphIt->second;
      for(map<string, Node *>::iterator refIt = node_A->referencedBy.begin();
          refIt != node_A->referencedBy.end();
          ++refIt) {
         Node *node_B = refIt->second;
         // node_B points to node_A
         if (node_B->outDegree != 0) {
            // node A is the row and node B is the col (calculating transpose)
            int row = node_A->id_num, col = node_B->id_num;
            // Update rowPtr if row jump happened.
            for (; last_row_seen <= row; last_row_seen++) {
               csr_matrix->csrRowPtrA[last_row_seen] = cur_index;
            }
            csr_matrix->csrValA[cur_index] = 1/(double)node_B->outDegree;
            csr_matrix->csrColIndA[cur_index] = col;
            cur_index++;
          }
      }
   }
   for (; last_row_seen < csr_matrix->width; last_row_seen++) {
      csr_matrix->csrRowPtrA[last_row_seen] = cur_index;
   }

   return csr_matrix;
}

double *createPrestigeVector(int width) {
   double init_val = 1/(double)width;
   double *prestige = (double *) check_malloc(width*sizeof(double));

   for (int i = 0 ; i < width; i++) {
      prestige[i] = init_val;
   }

   return prestige;
}
