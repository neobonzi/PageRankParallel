#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <map>
#include "Node.h"
#include "CSVReader.h"

#define CSV_DELIMITER ","

typedef map<string, Node *> NodeGraph;

using namespace std;
using namespace CSVReader;

int main(int argc, char **argv)
{
    string fileName = string(argv[1]);
    NodeGraph *graph = readCSVData(fileName);
    
    //Spit out the keys and values in map
    for(NodeGraph::iterator iter = graph->begin(); iter != graph->end(); ++iter)
    {
        string k = iter->first;
        Node *node = iter->second;
        
        cout << "Node: " << k << " Out " << node->outDegree << " In " << node->inDegree << endl;
        
        for(map<string, Node *>::iterator refIter = node->referencedBy.begin(); refIter != node->referencedBy.end(); ++refIter)
        {
            cout << "\t" << refIter->first << endl;
        }
    }
}

