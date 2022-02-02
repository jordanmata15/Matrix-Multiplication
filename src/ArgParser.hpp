#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <getopt.h>
#include <iostream>
#include <string>

#define C_USAGE 'h'
#define C_ROW 'r'
#define C_COL 'c'
#define C_QUIT 'q'

const std::string USAGE = "Required flags: \n"
                              "\t-c for # of columns\n" 
                              "\t-r for # of rows.\n"
                          "For example, \n"
                              "\t./program -r 3 -c 2\n"
                          "will create an array of 3 rows and 2 columns.\n";


class Arguments{
  public:
    int rows;
    int cols;
    Arguments():rows(-1), cols(-1){}
    bool doneParsing(){return rows==-1 || cols==-1;}
};

class ArgParser {
  private:
    void printUsage();
    void handleOption(char flag, char* optarg);
  public:
    Arguments* args;
    ArgParser() { args = new Arguments(); }
    ~ArgParser(){ free(args); }
    Arguments* parseArgs(int argc, char** argv);
    Arguments* getArgs(); // TODO
};

#endif // ARGPARSER_H
