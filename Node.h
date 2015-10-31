#include <vector>
#include <string>
#include <map>
using namespace std;

class Node
{
public:
   string identifier;
   string extraInfo;
   map<string, int> edgeWeights;
   vector<Node> referencedBy;
   double rank;
   int inDegree;
   double outDegree;

   Node(string id) : identifier(id), inDegree(0), outDegree(0) {}

   int compareTo(Node other)
   {
      if (rank < other.rank) return -1;
      else if (rank == other.rank) return 0;
      else return 1;
   }
};
