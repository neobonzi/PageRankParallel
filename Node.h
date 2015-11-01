#include <vector>
#include <string>
#include <map>
using namespace std;

// map of string identifier to a node
class Node
{
public:
   string identifier;
   vector<Node> referencedBy; // list of all nodes that point to it
   double prevRank;
   double curRank;
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
