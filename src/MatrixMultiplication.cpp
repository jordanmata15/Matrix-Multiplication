#include "MatrixMultiplication.hpp"



MatrixMultiplication::MatrixMultiplication(Matrix* aInput, 
                                           Matrix* bInput, 
                                           DataManager dm){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
  dataManager = dm;
  this->numThreads = numThreads;
}



MatrixMultiplication::MatrixMultiplication(Matrix* aInput, 
                                           Matrix* bInput){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
  dataManager = DataManager();
  this->numThreads = numThreads;
}



void MatrixMultiplication::setThreadTiles(int numThreads, int numSlices){
  this->numThreads = numThreads;
  this->numSlices = numSlices;
}



MatrixMultiplication::~MatrixMultiplication(){
  delete c;
}



void MatrixMultiplication::reinitializeC(){
  c->reinitialize(); // zero fill C
}



// leverages row major. Very good for C++.
Matrix* MatrixMultiplication::multiply_parallel(){
  int size, 
      rank,
      numSlices, 
      sliceWidth,
      a_rows = a->getNumRows(),
      a_cols = a->getNumCols(),
      b_rows = b->getNumRows(),
      b_cols = b->getNumCols(),
      c_cols = c->getNumCols();

  double* a_raw = a->getMatrix(),
        * b_raw = b->getMatrix(),
        * c_raw = c->getMatrix(),
        // pointers for each slice starting at our needed offsets
        * localA,
        * localB,
        * localC;

  MPI_Init(nullptr, nullptr);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  this->myRank = rank;
  numSlices = size;
  if (a_rows < numSlices) numSlices = a_rows;
  sliceWidth = ceil((double)a_rows/numSlices);

  // if we have excessive number of partitions, exit out of the ones we don't need
  if (rank >= size) return 0;

  for (int i = 0; i < a_rows; ++i)
    for (int j = 0; j < a_cols; ++j)
        a->setIJ(i,j,i*a_cols+j);


  for (int i = 0; i < b_rows; ++i)
    for (int j = 0; j < b_cols; ++j)
        b->setIJ(i,j,i*b_cols+j);

  dataManager.startTimer();

  // the row and index of our resulting matrix C
  int sliceStartIdx = rank*sliceWidth*a_cols;

  // pointers starting at our needed offsets
  localA = &a_raw[sliceStartIdx];
  localB = b_raw;
  localC = &c_raw[rank*sliceWidth*c_cols];

  #pragma omp parallel for num_threads(1)
  for (int i=0; i<sliceWidth; ++i){
    if (rank*sliceWidth+i >= a_rows) continue; // avoid reading past the end of the rows of A
    
    for (int k=0; k<a_cols; ++k){
      double r = localA[i*a_cols+k];
      
      for (int j=0; j<b_cols; ++j){
        localC[i*c_cols+j] += r * localB[k*b_cols+j];
      }
    }
  }
  
  this->consolidateC(rank, sliceWidth, numSlices, c_raw);
  dataManager.stopTimer();
  this->timeElapsedOptimal = dataManager.getTimeElapsed();

  MPI_Finalize();

  return c;
}



void MatrixMultiplication::consolidateC(int rank, int sliceWidth, int numSlices, double* c_raw){
  int startIdx = rank*sliceWidth*c->getNumCols(),
      receiveIdx;
  MPI_Status status;
  MPI_Datatype sliceType;
  MPI_Type_vector(sliceWidth, c->getNumCols(), c->getNumCols(), MPI_DOUBLE, &sliceType);
  MPI_Type_commit(&sliceType);

  if (rank>0)
    MPI_Send(&c_raw[startIdx], 1, sliceType, 0, 0, MPI_COMM_WORLD);

  if (rank==0){
    for (int sendingRank=1; sendingRank<numSlices; ++sendingRank){
      receiveIdx = sendingRank*sliceWidth*c->getNumCols();
      MPI_Recv(&c_raw[receiveIdx], 1, sliceType, sendingRank, 0, MPI_COMM_WORLD, &status);
    }
  }

  MPI_Type_free(&sliceType);
}



// leverages row major. Very good for C++.
Matrix* MatrixMultiplication::multiply(){
  int a_rows = a->getNumRows(),
      a_cols = a->getNumCols(),
      b_cols = b->getNumCols(),
      c_cols = c->getNumCols();
  
  /* same raw access as algorithm 0 */
  double* a_raw = a->getMatrix();
  double* b_raw = b->getMatrix();
  double* c_raw = c->getMatrix();

  
  dataManager.startTimer();

  #pragma omp parallel for num_threads(this->numThreads)
  for (int i=0; i<a_rows; ++i){
    for (int k=0; k<a_cols; ++k){
      double r = a_raw[i*a_cols+k];
      for (int j=0; j<b_cols; ++j){
        c_raw[i*c_cols+j] += r * b_raw[k*b_cols+j];
      }
    }
  }

  dataManager.stopTimer();
  this->timeElapsedSubOptimal = dataManager.getTimeElapsed();

  return c;
}

bool MatrixMultiplication::verify(Matrix* m){
  this->multiply(); // run multiplication and overwrite the old copy of C with a verified algorithm
  
  for (int i=0; i<c->getNumRows(); ++i)
    for (int j=0; j<c->getNumCols(); ++j)
       if (this->c->getIJ(i,j) != m->getIJ(i,j)) return false;
    
  return true;
}

double MatrixMultiplication::getTimeElapsedOptimal(){
  return this->timeElapsedOptimal;
}

double MatrixMultiplication::getTimeElapsedSubOptimal(){
  return this->timeElapsedSubOptimal;
}