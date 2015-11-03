#include <vector>
#include <string>
#include <map>

using namespace std;

namespace CSVReader
{

#define CSV_DELIMITER ","

typedef map<string, Node *> NodeGraph;

NodeGraph *readCSVData(string fileName)
{
    ifstream dataFile;
    string line;
    dataFile.open(fileName);
    NodeGraph *graph = new NodeGraph(); 

    if(dataFile.is_open())
    {
        while(getline(dataFile, line))
        {
            int delimIndex = line.find(CSV_DELIMITER);
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
                node1 = new Node(nodeID1);
                (*graph)[nodeID1] = node1;
            }
            
            // Skip
            line = line.substr(delimIndex + 1);
            delimIndex = line.find(CSV_DELIMITER);
           
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
                node2 = new Node(nodeID2);
                (*graph)[nodeID2] = node2;
            }
            
            // Make the node1 -> node2 relationship
            node1->outDegree++;
            node2->inDegree++;
            node2->addReferencedByNode(node1);
        }

        dataFile.close();
    }
    return graph;
}
}