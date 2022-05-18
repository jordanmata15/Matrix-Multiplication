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
    struct timeval startTime, endTime, elapsedTime;

  public:
    /**
     * Simple constructor.
     */
    DataManager();

    /**
     * Simple constructor.
     *
     * @param fileNameIn The name of the file to which we will read/write times
     *                   to. We may also delete the file entirely.
     */
    DataManager(std::string fileNameIn);
    
    /**
     * Start the timer object (gets current time).
     *
     */
    void startTimer();

    /**
     * Stop the timer object (gets current time).
     *
     */
    void stopTimer();

    /**
     * Get the time elapsed in seconds (with decimal) between start and stopping of the timer.
     *
     */
    double getTimeElapsed();
};

#endif // DATA_MANAGER_HPP
