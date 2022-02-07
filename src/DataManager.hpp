#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/time.h>

/**
 * Class used to make it easier to record and get statistics on the times each 
 * algorithm takes to run.
 */
class DataManager{
  
  private:
    int numAlgorithms;

    // Vectors to hold appropriate data for each algorithm. Each algorithm
    // is associated with an index according to it's index in fileNames
    // eg. fileNames[1], fileOutStreams[1], fileInStreams[1], times[1]
    //      are all associated with the same algorithm
    std::vector<std::string> fileNames;
    std::vector<std::ofstream*> fileOutStreams;
    std::vector<std::ifstream*> fileInStreams;
    std::vector<std::vector<double>> times;
    
    /**
     * Opens all the output files to prepare them for writing to.
     */
    void openOutputFiles();
    
    /**
     * Closes all output files and releases the memory used for their
     * output stream.
     */
    void closeOutputFiles();
    
    /**
     * Reads in a single file of times (in seconds) and adds them
     * to an internal vector.
     *
     * @param times The vector of times associated with a particular algorithm.
     * @param inputStream The input stream corresponding to that algorithm's 
     *        log file.
     */
    void readSingleAlgTimes(std::vector<double>* times, 
                            std::ifstream* inputStream);
    
    /**
     * Averages the values of a vector of doubles.
     *
     * @param timesVec The vector of times (in seconds).
     * @return The average of all the times.
     */
    double average(std::vector<double> timesVec);
    
    /**
     * Given a time struct, returns the value in seconds (with fractional 
     * values of seconds).
     *
     * @param tvElapsed The time value struct holding the elapsed time.
     * @return The decimal representation of how many seconds are held in 
     *         tvElapsed
     */
    double calculateElapsedSeconds(struct timeval* tvElapsed);

  public:
    /**
     * Simple constructor.
     *
     * @param fileNamesIn The list of filenames. Each corresponding to an 
     *        algorithm. Their relative index in this vector is their index 
     *        in the other vectors. Eg. fileNamesIn[1] corresponds to times[1]
     */
    DataManager(std::vector<std::string> fileNamesIn);
    

    /**
     * Writes the values for each algorithm to a separate file.
     */
    void writeTimesToFile();
    
    /**
     * Records the time for this algorithm (in seconds) to a vector.
     *
     * @param algNumber The index of the algorithm to save to the correct
     *        vector.
     * @param tvElapsed The struct holding the delta time taken for the 
     *        algorithm.
     */
    void recordTime(int algNumber, struct timeval* tvElapsed);
    

    /**
     * Reads times from a file and populates the vector of times. Reads from the
     * filenames passed in to the constructor.
     */
    void readTimes();
    
    /**
     * Deletes all files with the filenames passed in to the constructor.
     */
    void deleteFiles();
    
    /**
     * Calculates the average of a particular algorithm as given by the index.
     *
     * @param algNum the index of the algorithm within the times vector.
     * @return The average time of the specified algorithm (as held in times 
     *         vector).
     */
    double takeAverageOfAlg(int algNum);
};

#endif // DATA_MANAGER_HPP
