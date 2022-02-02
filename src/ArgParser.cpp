#include "ArgParser.hpp"

void ArgParser::printUsage(){
  std::cout << USAGE;
}


void ArgParser::handleOption( char flag, 
                              char*  optarg){
  switch(flag){
    case (C_ROW):
      args->rows = atoi(optarg);
      break;
    case(C_COL):
      args->cols = atoi(optarg);
      break;
    default:  
      printUsage();
  }
}


Arguments* ArgParser::parseArgs(int argc, char** argv){
  if (argc < 5){ 
    printUsage();
    exit(EXIT_FAILURE);
  }

  char flag;
  while( (flag=getopt(argc, argv, "r:c:")) != -1 ){
    handleOption(flag, optarg);
  }
  return args;
}

/*
int main(int argc, char** argv){
  ArgParser ap = ArgParser();
  ap.parseArgs(argc, argv);
  std::cout << ap.args->cols + ap.args->rows;
  return 0;
}
*/
