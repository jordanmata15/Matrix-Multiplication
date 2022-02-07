#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <iostream>
#include <string>

#define QUIT "q"
#define INVALID -1
#define NUM_ITERS 3 // runs for our average

/**
 * Arguments object to hold the values of the desired inputs given by the user.
 */
class Arguments{
  public:
    int rowsA;
    int colsA;
    int rowsB;
    int colsB;
    bool displayAB;
    bool displayC;
    bool displayAverages;
    int numRuns;

    /**
     * Basic constructor. sets default values.
     */
    Arguments():rowsA(INVALID), 
                colsA(INVALID), 
                rowsB(INVALID), 
                colsB(INVALID),
                displayAB(false), 
                displayC(false), 
                displayAverages(true), 
                numRuns(NUM_ITERS){}
};


/**
 * Class to continuously prompt the user for input and validate it. Fills an 
 * Arguments object with the users desired input.
 */
class ArgParser {

  private:
    /**
     * Validates an individual argument.
     *
     * @param value The value that is held by the argument. 
     * @return true if the program can accept this input, false otherwise.
     */
    bool validArgument(int value);

    /**
     * Prompts the user for input to populate the Arguments object.
     *
     * @param ptrToSet A pointer to the object we want the user to give a value
     *        for. Whatever integer input user the user enters will be stored in
     *        this variable.
     */
    void promptNumeric(int* ptrToSet);
   
    /**
     * Prompts the user for a y/n input and sets the stored variable to that 
     * value.
     * 
     * @param ptrToSet A pointer to the object we want the user to give a value
     *        for. If users input 'y', it will save true, 'q' exits the program,
     *        anything else defaults to false.
     */
    void promptYN(bool* ptrToSet);
    
    /**
     * Calls prompts for all optional values. Optional in the sense that they
     * have valid default values.
     */
    void promptOptional();
  
  public:
    Arguments* args;

    /**
     * Simple constructor that dynamically allocates memory for the arguments.
     */
    ArgParser() { args = new Arguments(); }

    /**
     * Simple destructor that frees the arguments once we're done with them.
     */
    ~ArgParser(){ delete args; }

    /**
     * Method used to verify all arguments are valid. For any invalid, it will
     * call prompt() to have the user try to populate them. If any arguments are
     * incorrect, ALL inputs are reset and the user is reprompted.
     *
     * @return A pointer to an argument structure from which the user inputs
     *         can be read.
     */
    Arguments* parseArgs();
};

#endif // ARG_PARSER_HPP
