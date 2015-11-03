#include <vector>
#include <string>
#include <map>
using namespace std;

// map of string identifier to a node
class Node
{
public:
   map<string, Node *> referencedBy;
   string identifier;
   double prevRank;
   double curRank;
   int inDegree;
   int outDegree;

   Node(string id) : identifier(id), inDegree(0), outDegree(0) {}
   
   // Addes a node to this nodes list of "referenced by" nodes
   void addReferencedByNode(Node *otherNode)
   {
      if(referencedBy.count(otherNode->identifier) == 0)
      {
         this->referencedBy[otherNode->identifier] = otherNode;
         this->inDegree++;
         otherNode->outDegree++;
      }
   }

   int compareTo(Node other)
   {
      if (curRank < other.curRank) return -1;
      else if (curRank == other.curRank) return 0;
      else return 1;
   }
};
