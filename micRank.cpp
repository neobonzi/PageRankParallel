#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "Node.h"
#include "GraphUtils.h"
#include "CSVReader.h"
#include "SNAPReader.h"

#define CSV_DELIMITER ","

using namespace std;
using namespace CSVReader;
using namespace SNAPReader;

int main(int argc, char **argv)
{
    string fileName = string(argv[1]);
    GraphUtils:: NodeGraph *graph;

   	//Check the filename to see if its SNAP or CSV
   	if(fileName.substr(fileName.find_last_of(".") + 1) == "csv")
   	{
    	graph = readCSVData(fileName);
   	}
   	else
   	{
   		graph = readSNAPData(fileName);
   	}

    GraphUtils::printNodeGraph(graph);
}

