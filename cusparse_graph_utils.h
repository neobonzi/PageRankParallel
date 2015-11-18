#include "GraphUtils.h"

class CSRMatrix {
public:
   vector<Node *> nodes;
   int width;
   int nnz; // number of nonzero elements in the matrix
   double *csrValA; // holds all nonzero values of A
   int *csrRowPtrA; // holds row indicies in compressed CSR format
   int *csrColIndA; // holds column indicies of each csrValA item.

   CSRMatrix(int w);
   void setNnz(int val);
};

void *check_malloc(size_t size);
CSRMatrix *listToCSRMatrix(GraphUtils::NodeGraph *node_graph);
double *createPrestigeVector(int width);
