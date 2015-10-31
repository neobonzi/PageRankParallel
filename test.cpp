#include "Node.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
   Node node1 = Node("node1");
   node1.inDegree = 1;
   Node node2 = Node("node2");
   node2.inDegree = 2;

   cout << node1.compareTo(node2) << endl;
}

