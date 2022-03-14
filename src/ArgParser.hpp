#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <iostream>
#include <getopt.h>

#define DEFAULT_THREADS 1
#define DEFAULT_ALG_NUM 2
#define NUM_ALGORITHMS 3
#define INVALID -1
#define USAGE "\nusage: ./MonteCarloApproximation -M rowsA -N colsA/colsB -P colsB <optional_flags>\n"\
              "\tMandatory flags with arguments:\n"\
              "\t\t-M reads in rowsA\t\t(rowsA \t\t> 0, integer)\n"\
              "\t\t-N reads in colsA/colsB\t\t(colsA, colsB \t> 0, integer)\n"\
              "\t\t-P reads in colsB\t\t(colsB \t\t> 0, integer)\n"\
              "\tOptional flags with arguments:\n"\
              "\t\t-n reads in NUM_THREADS\t\t(NUM_THREADS \t> 0, integer)\n"\
              "\t\t-p reads in ALG_NUM\t(ALG_NUM either 0, 1, or 2)\n"\
              "\tOptional flags without arguments:\n"\
              "\t\t-a enables displaying matrices A/B one time each.\n"\
              "\t\t-c enables displaying matrix C one time for each algorithm.\n"

/**
 * @brief Arguments object to hold the values of the desired inputs given by the user.
 */
class Arguments{
  public:
    int algNum;
    int numThreads;
    int rowsA;
    int colsA;
    int rowsB;
    int colsB;
    bool displayAB;
    bool displayC;
    
    /**
     * Basic constructor. sets default values.
     */
    Arguments():algNum(DEFAULT_ALG_NUM),
                numThreads(DEFAULT_THREADS),
                rowsA(INVALID), 
                colsA(INVALID), 
                rowsB(INVALID), 
                colsB(INVALID),
                displayAB(false), 
                displayC(false){}
};


/**
 * @brief Class to read in command line arguments and validate them. Fills an Arguments 
 *        object with the users desired input.
 */
class ArgParser {

  private:
    /**
     * @brief Reads in an string and returns the converted integer.
     * 
     * @param value String value to convert to an integer. 
     * @return int The converted integer. Returns -1 if the string was not successfully converted.
     */
    int readInt(char flag, char* value);

    /**
     * @brief Prints out the usage of our program.
     * 
     */
    void printUsage(){ std::cout << USAGE << std::endl; }

    /**
     * @brief Validates whether our inputs are valid and we can populate the matrices.
     * 
     * @return true if the populated arguments are valid.
     * @return false otherwise.
     */
    bool validArgs();
  
  public:
    Arguments* args;

    /**
     * @brief Construct a new Arg Parser object. Dynamically allocate the memory.
     * 
     */
    ArgParser() { args = new Arguments(); }

    /**
     * @brief Destroy the Arg Parser object and free the allocated memory.
     * 
     */
    ~ArgParser(){ delete args; }

    /**
     * @brief Method used to parse the flags from the user. Validates integer arguments
     * and exits if invalid.
     * 
     * @param argc The argument count provided to main().
     * @param argv The argument list provided to main().
     * @return Arguments* A pointer to an argument structure from which the user inputs
     *         can be read.
     */
    Arguments* parseArgs(int argc, char** argv);
};

#endif // ARG_PARSER_HPP
