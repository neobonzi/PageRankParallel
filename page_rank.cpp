#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "PageRank.h"
#include "Node.h"
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

   pageRank(graph);
   return 0;
}

