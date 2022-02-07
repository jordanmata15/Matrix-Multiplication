#include "DataManager.hpp"


// simple constructor
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


// open files we write out times to
void DataManager::openOutputFiles(){
  int i=0;
  for (auto fname:fileNames){
    std::ofstream* s = new std::ofstream(fname);
    fileOutStreams.at(i) = s;
    ++i;
  }
}


// close files we write out times to
void DataManager::closeOutputFiles(){
  for (auto s:fileOutStreams){
    s->close();
    delete s;
  }
}


// deletes files with filenames in the vector passed into the constructor
void DataManager::deleteFiles(){
  for (auto fname:fileNames){
    std::remove(fname.c_str());
  }
}


/* read in the times for all algorithms from a file and populate the appropriate
 * vector */
void DataManager::readTimes(){
  int i=0;
  for (auto fname:fileNames){
    std::ifstream* s = new std::ifstream(fname);
    readSingleAlgTimes(&times.at(i), s);
    s->close();
    delete s;
    ++i;
  }
}


/* Read a specific input stream line by line into the vector.
 */
void DataManager::readSingleAlgTimes(std::vector<double>* times, 
                                     std::ifstream* inputStream){
  double time;
  std::string line;

  while( std::getline(*inputStream, line) ){
    time = std::stod(line.c_str());
    times->push_back(time);
  }
}


// write out all algorithms to their specified files
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


// push the time to the vector corresponding to this algorithm
void DataManager::recordTime(int algNum, struct timeval* tvElapsed){
  double elapsedMicro = calculateElapsedSeconds(tvElapsed);
  times.at(algNum).push_back(elapsedMicro);
}


// get the average of a particular algorithm's elapsed times
double DataManager::takeAverageOfAlg(int algNum){
  return average(times.at(algNum));
}


// average the values in the provided vector
double DataManager::average(std::vector<double> timesVec){
  long double sum = 0; // use a long double to avoid overflow
  if (timesVec.size() == 0){
    return 0;
  }
  for (auto i:timesVec){
    sum += i;
  }
  return (double) sum/timesVec.size();
}


// convert a timeval struct holding the delta time to a numerical (double) value
double DataManager::calculateElapsedSeconds(struct timeval* tvElapsed){
  // convert to microseconds and back to force everything after the decimal to remain
	double elapsedSecondsMicro = tvElapsed->tv_sec*1000000 + (tvElapsed->tv_usec);
	double elapsedSeconds = elapsedSecondsMicro /= 1000000;
  return elapsedSeconds;
}
