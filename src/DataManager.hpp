#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <sys/time.h>

#define OUT_FILE1 "./algorithm1.log"
#define OUT_FILE2 "./algorithm2.log"
#define OUT_FILE3 "./algorithm3.log"

class DataManager{
  private:
    int numAlgorithms;
    std::vector<std::string> fileNames;
    std::vector<std::ofstream*> fileOutStreams;
    std::vector<std::ifstream*> fileInStreams;
    std::vector<std::vector<double>> times; 
    void openOutputFiles();
    void closeOutputFiles();
    void readSingleAlgTimes(std::vector<double>* times, std::ifstream* inputStream);
    double average(std::vector<double> intVec);
    double calculateElapsedSeconds(struct timeval* tv_elapsed);

  public:
    DataManager(std::vector<std::string> fileNamesIn);
    void writeTimesToFile();
    void recordTime(int streamNumber, struct timeval* tv_elapsed);
    void readTimes();
    void deleteFiles();
    double takeAverageOfAlg(int algNum);
};

#endif // DATA_MANAGER_HPP
