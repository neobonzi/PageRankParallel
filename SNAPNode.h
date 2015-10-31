#include <vector>

using namespace std;

class SNAPNode
{
   int identifier;
   int edges;
public:
   vector<int> inEdges;
   double pageRank;

   // Constructors
   SNAPNode() : pageRank(1.0) {}
   SNAPNode(int id) : identifier(id), pageRank(1.0) {}
   SNAPNode(double rank) : pageRank(rank) {}

   // Methods
   int compareTo(SNAPNode other)
   {
      if (other.pageRank < pageRank)
         return -1;
      else
         return 1;
   }
};
