#include "DataManager.hpp"

// simple constructor
DataManager::DataManager(){
  times = std::vector<double>();
}

// simple constructor
DataManager::DataManager(std::string fileNameIn){
  fileName = fileNameIn;
  times = std::vector<double>();
}


// read in the times for the algorithm from a file to our times vector
void DataManager::readTimes(){
  std::ifstream* inputStream = new std::ifstream(fileName);
  double time;
  std::string line;
  
  while( std::getline(*inputStream, line) ){
    time = std::stod(line.c_str());
    times.push_back(time);
  }
  
  inputStream->close();
  delete inputStream;
}


// write out times of the algorithm to it's specified file
void DataManager::writeTimesToFile(){
  std::remove(fileName.c_str());
  fileOutStream = new std::ofstream(fileName);
  for (auto time:times){
    *fileOutStream << time << std::endl;
  }
  fileOutStream->close();
  delete fileOutStream;
}


// push the time to the vector corresponding to this algorithm
void DataManager::recordTime(struct timeval* tvElapsed){
  double elapsedMicro = calculateElapsedSeconds(tvElapsed);
  times.push_back(elapsedMicro);
}


// average the values in the provided vector
double DataManager::average(){
  long double sum = 0; // use a long double to avoid overflow
  if (times.size() == 0){
    return 0;
  }
  for (auto i:times){
    sum += i;
  }
  return (double) sum/times.size();
}


// convert a timeval struct holding the delta time to a numerical (double) value
double DataManager::calculateElapsedSeconds(struct timeval* tvElapsed){
  // convert to microseconds and back to retain the less significant bits
	double elapsedSecondsMicro = tvElapsed->tv_sec*1000000 + (tvElapsed->tv_usec);
	double elapsedSeconds = elapsedSecondsMicro /= 1000000;
  return elapsedSeconds;
}
