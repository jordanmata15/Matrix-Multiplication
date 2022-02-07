#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <getopt.h>
#include <iostream>
#include <sstream>
#include <string>

#define QUIT "q"

class Arguments{
  public:
    int rowsA;
    int colsA;
    int rowsB;
    int colsB;
    Arguments():rowsA(-1), colsA(-1), rowsB(-1), colsB(-1){}
};

class ArgParser {
  private:
    bool matrixARead = false;
    bool matrixBRead = false;
    bool validInput();
  public:
    Arguments* args;
    ArgParser() { args = new Arguments(); }
    ~ArgParser(){ delete args; }
    void prompt(int* ptrToSet);
    Arguments* parseArgs();
};

#endif // ARG_PARSER_HPP
