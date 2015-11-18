#ifndef SNAP_READER_H_
#define SNAP_READER_H_
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include "GraphUtils.h"

using namespace std;

namespace SNAPReader
{

#define SNAP_DELIMITER "\t"
#define SNAP_COMMENT '#'

GraphUtils::NodeGraph *readSNAPData(char *fileName)
{
    int id_num = 0;
    ifstream dataFile;
    string line;
    dataFile.open(fileName);
    GraphUtils::NodeGraph *graph = new GraphUtils::NodeGraph(); 

    if(dataFile.is_open())
    {
        while(getline(dataFile, line))
        {
            //Check or comment
            if (line[0] == SNAP_COMMENT)
            {
                continue;
            }

            int delimIndex = line.find(SNAP_DELIMITER);
            string nodeID1, nodeID2;
            
            nodeID1.assign(line.substr(0, delimIndex));
            Node *node1;
            
            // Look up the identifier in the map
            if (graph->count(nodeID1) != 0)
            {
                node1 = graph->at(nodeID1);
            }
            else
            {   
                node1 = new Node(nodeID1, id_num++);
                (*graph)[nodeID1] = node1;
            }

            // Get neighbor 
            line = line.substr(delimIndex + 1);
            delimIndex = line.find(CSV_DELIMITER);
            nodeID2.assign(line.substr(0, delimIndex));
            Node *node2;
            
            // Create the neighbor node if it doesnt exist
            // and put it in the map
            if (graph->count(nodeID2) != 0)
            {
                node2 = graph->at(nodeID2);
            }
            else
            {
                node2 = new Node(nodeID2, id_num++);
                (*graph)[nodeID2] = node2;
            }
            
            // Make the node1 -> node2 relationship
            node2->addReferencedByNode(node1);
        }

        dataFile.close();
    }
    else {
      perror("SNAPReader");
      exit(-1);
    }
    return graph;
}
}
#endif
