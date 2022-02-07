#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <iostream>
#include <string>

#define QUIT "q"
#define INVALID -1

/**
 * Arguments object to hold the values of the desired inputs given by the user.
 */
class Arguments{
  public:
    int rowsA;
    int colsA;
    int rowsB;
    int colsB;
    Arguments():rowsA(INVALID), colsA(INVALID), rowsB(INVALID), colsB(INVALID){}
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
     * Promts the user for input to populate the Arguments object.
     *
     * @param ptrToSet A pointer to the object we want the user to give a value
     *        for. Whatever integer input user the user enters will be stored in
     *        this variable.
     */
    void prompt(int* ptrToSet);

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
