#include "DataManager.hpp"
#include <iostream>


DataManager::DataManager(std::vector<std::string> fileNamesIn){
  fileNames = fileNamesIn;
  numAlgorithms = fileNamesIn.size();
  fileOutStreams = std::vector<std::ofstream*>(numAlgorithms);
  fileInStreams = std::vector<std::ifstream*>(numAlgorithms);
  times = std::vector<std::vector<long int>>(numAlgorithms);

  for (auto v:times){
    v = std::vector<long int>();
  }
  readTimes();  
  deleteFiles();
  openFiles();
}

DataManager::~DataManager(){
  for (auto s:fileOutStreams){
    s->close();
    delete s;
  }
}


void DataManager::openFiles(){
  int i=0;
  for (auto fname:fileNames){
    std::ofstream* s = new std::ofstream(fname);
    fileOutStreams.at(i) = s;
    i++;
  }
}

void DataManager::deleteFiles(){
  for (auto fname:fileNames){
    std::remove(fname.c_str());
  }
}


void DataManager::readTimes(){
  int i=0;
  for (auto fname:fileNames){
    std::ifstream* s = new std::ifstream(fname);
    fileInStreams.at(i) = s;
    i++;
  }

  for (int i=0; i<3; i++){
    readSingleAlgTimes(i);
  }

  for (auto s:fileInStreams){
    s->close();
    delete s;
  }
}

void DataManager::readSingleAlgTimes(int algNum){
  int time;
  std::string line;
  std::ifstream* s = fileInStreams.at(algNum);
  while( std::getline(*s, line) ){
    time = std::stoi(line.c_str());
    times.at(algNum).push_back(time);
  }
}

void DataManager::writeTime(int streamNumber, long int time){
  int algNum = streamNumber-1;
  times.at(algNum).push_back(time);
  *(fileOutStreams.at(streamNumber-1)) << time << std::endl;
}

long int DataManager::takeAverageOfAlg(int algNum){
  return average(times.at(algNum-1));
}

long int DataManager::average(std::vector<long int> intVec){
  long int sum;
  if (intVec.size() == 0){
    return -1;
  }

  for (auto i:intVec){
    sum += i;
  }
  sum /= (long int) intVec.size();
  return sum;
  //return sum/(long int)intVec.size();
}
