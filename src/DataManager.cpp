#include "DataManager.hpp"
#include <iostream>


DataManager::DataManager(std::vector<std::string> fileNamesIn){
  fileNames = fileNamesIn;
  numAlgorithms = fileNamesIn.size();
  fileOutStreams = std::vector<std::ofstream*>(numAlgorithms);
  fileInStreams = std::vector<std::ifstream*>(numAlgorithms);
  times = std::vector<std::vector<double>>(numAlgorithms);

  for (auto v:times){
    v = std::vector<double>();
  }
}

DataManager::~DataManager(){
}

void DataManager::openOutputFiles(){
  int i=0;
  for (auto fname:fileNames){
    std::ofstream* s = new std::ofstream(fname);
    fileOutStreams.at(i) = s;
    i++;
  }
}

void DataManager::closeOutputFiles(){
  for (auto s:fileOutStreams){
    s->close();
    delete s;
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
  double time;
  std::string line;
  std::ifstream* s = fileInStreams.at(algNum);
  while( std::getline(*s, line) ){
    time = std::stod(line.c_str());
    times.at(algNum).push_back(time);
  }
}


void DataManager::writeTimesToFile(){
  deleteFiles();
  openOutputFiles();

  for (int i=0; i<numAlgorithms; ++i){
    for (auto time:times.at(i)){
      *(fileOutStreams.at(i)) << time << std::endl;
    }
  }

  closeOutputFiles();
}


void DataManager::recordTime(int streamNumber, struct timeval* tv_elapsed){
  int algNum = streamNumber-1;
  double elapsedMicro = calculateElapsedSeconds(tv_elapsed);
  times.at(algNum).push_back(elapsedMicro);
}


double DataManager::takeAverageOfAlg(int algNum){
  return average(times.at(algNum-1));
}


double DataManager::average(std::vector<double> intVec){
  long double sum = 0;
  if (intVec.size() == 0){
    return -1;
  }

  for (auto i:intVec){
    sum += i;
  }
  sum /=  intVec.size();
  return (double) sum;
}


double DataManager::calculateElapsedSeconds(struct timeval* tv_elapsed){
  // convert to microseconds and back to force everything after the decimal to remain
	double elapsedSecondsMicro = tv_elapsed->tv_sec*1000000 + (tv_elapsed->tv_usec);
	double elapsedSeconds = elapsedSecondsMicro /= 1000000;
  return elapsedSeconds;
}
