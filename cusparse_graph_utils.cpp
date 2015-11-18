#include "cusparse_graph_utils.h"
#include <algorithm>

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
   for (int i = 0; i < w; i++) {
      csrRowPtrA[i] = 0;
   }
   nnz = 0;
}

void CSRMatrix::setNnz(int val) {
   nnz = val;
   csrValA = (double *) malloc(nnz*sizeof(double));
   csrColIndA = (int *) malloc(nnz*sizeof(int));
   csrRowPtrA[width] = val;
}

class CSRItem {
public:
   int row, col;
   float val;
   CSRItem(int r, int c, int v) : row(r), col(c), val(v) {}
   friend class CompareByRow;
   class CompareByRow {
      public:
      bool operator()(const CSRItem a, const CSRItem b) {
         return a.row < b.row;
      }
   };
};

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
   std::sort(csr_matrix->nodes.begin(), csr_matrix->nodes.end(),
             Node::CompareById());

   // Set nnz value and allocate val and col matrices
   csr_matrix->setNnz(nnz);

   vector<CSRItem> items;
   // Fill in values;
   for (NodeGraph::iterator graphIt = node_graph->begin();
        graphIt != node_graph->end();
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
            items.push_back(CSRItem(row, col, 1/(double)node_B->outDegree));
          }
      }
   }
   std::sort(items.begin(), items.end(), CSRItem::CompareByRow());
   int rowNdx = 0, i;
   printf("size: %d\n", items.size());
   printf("items vector: ");
   for (i = 0; i < items.size(); ++i) {
      printf("%d ", items[i].row);
      csr_matrix->csrValA[i] = items[i].val;
      csr_matrix->csrColIndA[i] = items[i].col;
      for (; rowNdx <= items[i].row; ++rowNdx) {
         csr_matrix->csrRowPtrA[rowNdx] = i;
      }
   }
   printf("\n");

   --i;
   for (; rowNdx < csr_matrix->width; ++rowNdx) {
      csr_matrix->csrRowPtrA[rowNdx] = i;
   }

   printf("Row: ");
   for (int i = 0; i < csr_matrix->width + 1; i++) {
      printf("%d ", csr_matrix->csrRowPtrA[i]);
   }
   printf("\n");

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
