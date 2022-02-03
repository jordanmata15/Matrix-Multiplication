#include "ArgParser.hpp"


void ArgParser::prompt(int* ptrToSet){
  std::string userInput;
  std::cout << "Please enter an integer >0. "
               "Enter '" << QUIT << "' to quit." << std::endl;
  getline(std::cin, userInput);
  if (userInput == QUIT){
    exit(0);
  }
  try{
    *ptrToSet = std::stoi(userInput);
  }
  catch(std::exception &e){
    std::cout << "Invalid input! Please try again." << std::endl;
    *ptrToSet = -1;
  }
}


Arguments* ArgParser::parseArgs(){
  while (1){
    if (args->rowsA <= 0){
      std::cout << "\nNumber of ROWS for matrix A. " << std::endl;
      prompt(&(args->rowsA));
    }
    else if (args->colsA <= 0){
      std::cout << "\nNumber of COLUMNS for matrix A: " << std::endl;
      prompt(&(args->colsA));
    }
    else if (args->rowsB <= 0){
      std::cout << "\nNumber of ROWS for matrix B: " << std::endl;
      prompt(&(args->rowsB));
    }
    else if (args->colsB <= 0){
      std::cout << "\nNumber of COLUMNS for matrix B: " << std::endl;
      prompt(&(args->colsB));
    }
    else if (args->colsA != args->rowsB){
      std::cout << "\nIncompatible matrix dimensions: \n"
                   "A (" << args->rowsA << " x " << args->colsA << ") and \n"
                   "B (" << args->rowsB << " x " << args->colsB << ")" << std::endl;
      args->rowsA = -1;
      args->rowsB = -1; 
      args->colsA = -1;
      args->colsB = -1;
    }
    else{
      std::cout << std::endl;
      break;
    }
  }
  return args;
}
