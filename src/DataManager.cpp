#include "DataManager.hpp"

// simple constructor
DataManager::DataManager(){
  
}


void DataManager::startTimer(){
  gettimeofday(&this->startTime, NULL);
}


void DataManager::stopTimer(){
  gettimeofday(&this->endTime, NULL);
}


double DataManager::getTimeElapsed(){
  timersub(&this->endTime, &this->startTime, &this->elapsedTime);

  // convert to microseconds and back to retain the less significant bits
	double elapsedSecondsMicro = this->elapsedTime.tv_sec*1000000 + (this->elapsedTime.tv_usec);
	double elapsedSeconds = elapsedSecondsMicro /= 1000000;
  return elapsedSeconds;
}

