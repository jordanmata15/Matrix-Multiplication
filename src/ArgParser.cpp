#include "ArgParser.hpp"

int ArgParser::readInt(char flag, char* value){
  // https://stackoverflow.com/questions/46208837/c-using-isdigit-to-check-if-optarg-is-a-digit
  char *end;
  errno = 0;
  int intValue = strtol(value, &end, 10);
  while (isspace(*end)) ++end;
  if (errno || *end) {
      return -1;
  }
  return intValue;
}

bool ArgParser::validArgs(){
  return  args->rowsA > 0 &&
          args->colsA > 0 &&
          args->rowsB > 0 &&
          args->colsB > 0;
}

Arguments* ArgParser::parseArgs(int argc, char** argv){
  int option;

  while ( (option=getopt(argc,argv,"M:N:P:t:")) != -1 ){
    switch(option){
      case 't':
        args->numThreads = this->readInt(option, optarg);
        if (args->numThreads <= 0){
          fprintf(stderr,"Flag -%c expects an integer input greater than 0. Found: '%s'\n", option, optarg);
          printUsage();
          exit(1);
        }
        break;
      
      case 'M':
        args->rowsA = this->readInt(option, optarg);
        if (args->rowsA <= 0){
          fprintf(stderr,"Flag -%c expects an integer input greater than 0. Found: '%s'\n", option, optarg);
          printUsage();
          exit(1);
        }
        break;

      case 'N':
        args->colsA = this->readInt(option, optarg);
        args->rowsB = args->colsA;
        if (args->colsA <= 0){
          fprintf(stderr,"Flag -%c expects an integer input greater than 0. Found: '%s'\n", option, optarg);
          printUsage();
          exit(1);
        }
        break;
      
      case 'P':
        args->colsB = this->readInt(option, optarg);
        if (args->colsB <= 0){
          fprintf(stderr,"Flag -%c expects an integer input greater than 0. Found: '%s'\n", option, optarg);
          printUsage();
          exit(1);
        }
        break;

      case '?':
        printUsage();
        exit(1);
    }
  }
  if (!validArgs()){
    printUsage();
    exit(1);
  }
  return args;
}