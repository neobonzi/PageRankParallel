#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "PageRank.h"
#include "Node.h"
#include "GraphUtils.h"
#include "CSVReader.h"
#include "SNAPReader.h"
#include "PageRankUtils.h"

#define CSV_DELIMITER ","

using namespace std;
using namespace CSVReader;
using namespace SNAPReader;
using namespace PageRankUtils;

int main(int argc, char **argv)
{
   string fileName = string(argv[1]);
   GraphUtils::NodeGraph *graph;

   //Check the filename to see if its SNAP or CSV
   if(fileName.substr(fileName.find_last_of(".") + 1) == "csv")
   {
      graph = readCSVData(argv[1]);
   }
   else
   {
      graph = readSNAPData(argv[1]);
   }

   // pageRank function call
   // We can use the same function header for all of the implementations
   // of pageRank and then just link in the appropriate cpp files at compile
   // time so we can have the same main function for all of them.
   // for nvcc I included the --x flag to force the .cpp file to be compiled
   // as a cuda file.
   pageRank(graph);
   return 0;
}

