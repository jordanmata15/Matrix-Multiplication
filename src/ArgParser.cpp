#include "ArgParser.hpp"


bool ArgParser::validArgument(int value){
  if (value <= 0) return false;
  else return true;
}


void ArgParser::promptNumeric(int* ptrToSet){
  std::string userInput;
  std::cout << "Please enter an integer > 0. Enter '" << QUIT << 
                "' to quit." << std::endl;
  
  getline(std::cin, userInput);
  if (userInput == QUIT){ exit(0); }
  try{ *ptrToSet = std::stoi(userInput); }
  catch(std::exception &e){
    std::cout << "Invalid input!" << std::endl;
    *ptrToSet = INVALID;
  }
}


void ArgParser::promptYN(bool* ptrToSet){
  std::string userInput;
  std::cout << "Please enter y for yes. Enter '" << QUIT << "' to quit. " 
                "Everything else defaults to n (no)."<< std::endl;
  
  getline(std::cin, userInput);
  if (userInput == QUIT) { exit(0); }
  if (userInput.compare("y") == 0) { *ptrToSet = true; }
  else { *ptrToSet = false;}
}


void ArgParser::promptOptional(){
  std::cout << "\nDisplay matrices A/B after generating them? (y/n)\n";
  promptYN( &(args->displayAB) );
  std::cout << "\nDisplay matrix C after each algorithm? (y/n)\n";
  promptYN( &(args->displayC) );
  std::cout << "\nDisplay average time elapsed for each algorithm? (y/n)\n";
  promptYN( &(args->displayAverages) );
  
  if (args->displayAverages){  
    std::cout << "\nHow many iterations to be averaged? "
                    "Invalid input defaults to 10.\n";
    promptNumeric( &(args->numRuns) );
    if (args->numRuns == INVALID) 
        args->numRuns = NUM_ITERS;
  }
}


Arguments* ArgParser::parseArgs(){
  // continuously prompt while all arguments are not valid
  while (1){
    if ( !validArgument(args->rowsA) ){
      std::cout << "\nNumber of ROWS for matrix A. " << std::endl;
      promptNumeric(&(args->rowsA));
    }
    else if ( !validArgument(args->colsA) ){
      std::cout << "\nNumber of COLUMNS for matrix A: " << std::endl;
      promptNumeric(&(args->colsA));
    }
    else if ( !validArgument(args->rowsB) ){
      std::cout << "\nNumber of ROWS for matrix B: " << std::endl;
      promptNumeric(&(args->rowsB));
    }
    else if ( !validArgument(args->colsB) ){
      std::cout << "\nNumber of COLUMNS for matrix B: " << std::endl;
      promptNumeric(&(args->colsB));
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
      break;
    }
  }

  promptOptional();
  
  return args;
}
