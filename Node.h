#ifndef NODE_H_
#define NODE_H_
#include <vector>
#include <string>
#include <map>
using namespace std;

// map of string identifier to a node
class Node
{
public:
   map<string, Node *> referencedBy;
   int id_num;
   string identifier;
   double prevRank;
   double curRank;
   int inDegree;
   int outDegree;

   Node(string id, int id_num) : identifier(id), id_num(id_num),
                                 inDegree(0), outDegree(0) {}
   
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

   void updatePrestige(double newPrestige)
   {
       this->prevRank = this->curRank;
       this->curRank = newPrestige;
   }

   int compareTo(Node other)
   {
      if (curRank < other.curRank) return -1;
      else if (curRank == other.curRank) return 0;
      else return 1;
   }

   friend class CompareByRank;
   class CompareByRank {
      public:
      bool operator()(const Node *a, const Node *b) {
         return a->curRank < b->curRank;
      }
   };
};
#endif
