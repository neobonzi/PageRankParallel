#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_
#include <cstdlib>
#include <iostream>
#include <iterator>

#include "Node.h"

using namespace std;

namespace GraphUtils
{

typedef map<string, Node *> NodeGraph;

void printNodeGraph(NodeGraph *graph)
{
    //Spit out the keys and values in map
    for(NodeGraph::iterator iter = graph->begin(); iter != graph->end(); ++iter)
    {
        string k = iter->first;
        Node *node = iter->second;
        cout << "Node: " << iter->first;
        cout << " Out " << node->outDegree << " In " << node->inDegree << endl;
        
        for(map<string, Node *>::iterator refIter = node->referencedBy.begin(); refIter != node->referencedBy.end(); ++refIter)
        {
            cout << "\t" << refIter->first << endl;
        }
    }
}
}
#endif