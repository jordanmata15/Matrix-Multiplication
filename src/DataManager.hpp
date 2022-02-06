#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <fstream>
#include <string>
#include <vector>

class DataManager{
  private:
    int numAlgorithms;
    std::vector<std::string> fileNames;
    std::vector<std::ofstream*> fileOutStreams;
    std::vector<std::ifstream*> fileInStreams;
    std::vector<std::vector<long int>> times; 
    void openFiles();
    void readSingleAlgTimes(int algNum);
    long int average(std::vector<long int> intVec);

  public:
    DataManager(std::vector<std::string> fileNamesIn);
    ~DataManager();
    void writeTime(int streamNumber, long int time);
    void readTimes();
    void deleteFiles();
    long int takeAverageOfAlg(int algNum);
};

#endif // DATA_MANAGER_HPP
