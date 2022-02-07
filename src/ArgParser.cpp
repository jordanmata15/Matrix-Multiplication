#include "ArgParser.hpp"


bool ArgParser::validArgument(int value){
  if (value <= 0) return false;
  else return true;
}


void ArgParser::prompt(int* ptrToSet){
  std::string userInput;
  std::cout << "Please enter an integer > 0. "
               "Enter '" << QUIT << "' to quit." << 
               std::endl;
  getline(std::cin, userInput);
  
  if (userInput == QUIT){
    exit(0);
  }
  try{
    *ptrToSet = std::stoi(userInput);
  }
  catch(std::exception &e){
    std::cout << "Invalid input! Please try again." << std::endl;
    *ptrToSet = INVALID;
  }
}


Arguments* ArgParser::parseArgs(){
  // continuously prompt while all arguments are not valid
  while (1){
    if ( !validArgument(args->rowsA) ){
      std::cout << "\nNumber of ROWS for matrix A. " << std::endl;
      prompt(&(args->rowsA));
    }
    else if ( !validArgument(args->colsA) ){
      std::cout << "\nNumber of COLUMNS for matrix A: " << std::endl;
      prompt(&(args->colsA));
    }
    else if ( !validArgument(args->rowsB) ){
      std::cout << "\nNumber of ROWS for matrix B: " << std::endl;
      prompt(&(args->rowsB));
    }
    else if ( !validArgument(args->colsB) ){
      std::cout << "\nNumber of COLUMNS for matrix B: " << std::endl;
      prompt(&(args->colsB));
    }
    else if (args->colsA != args->rowsB){
      std::cout << "\nIncompatible matrix dimensions: \n"
                   "A (" << args->rowsA << " x " << args->colsA << ") and \n"
                   "B (" << args->rowsB << " x " << args->colsB << ")" << 
                   std::endl;
      // if anything goes wrong, reset everything
      args->rowsA = args->rowsB 
                  = args->colsA 
                  = args->colsB 
                  = INVALID;
    }
    else{ // All input is validated to be correct
      std::cout << std::endl;
      break;
    }
  }
  return args;
}
