#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <fstream>
#include <vector>
#include <sys/time.h>

/**
 * Class used to store times for a specific algorithm and generate statistics on
 * the set of time data.
 */
class DataManager{
  
  private:
    std::string fileName;
    std::ofstream* fileOutStream;
    std::vector<double> times;
    
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
     * @param fileNameIn The name of the file to which we will read/write times
     *                   to. We may also delete the file entirely.
     */
    DataManager(std::string fileNameIn);
    

    /**
     * Writes all recorded elapsed times (in seconds) of this algorithm.
     */
    void writeTimesToFile();
   

    /**
     * Records the time for this algorithm (in seconds) to a vector.
     *
     * @param tvElapsed The struct holding the delta time taken for the 
     *        algorithm.
     */
    void recordTime(struct timeval* tvElapsed);
    

    /**
     * Reads times from a file and populates the vector of times. Reads from the
     * filename passed in to the constructor.
     */
    void readTimes();
    
    
    /**
     * Averages the values of the recorded times (in seconds).
     *
     * @return The average of all the times taken by the algorithm represented
     *         by this DataManager.
     */
    double average();
    
};

#endif // DATA_MANAGER_HPP
