#include "MatrixMultiplication.hpp"



MatrixMultiplication::MatrixMultiplication(Matrix* aInput, 
                                           Matrix* bInput, 
                                           DataManager dm){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
  dataManager = dm;
  reinitializeC();
}



MatrixMultiplication::MatrixMultiplication(Matrix* aInput, 
                                           Matrix* bInput){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
  dataManager = DataManager();
  reinitializeC();
}



void MatrixMultiplication::setThreads(int numThreads){
  this->numThreads = numThreads;
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
      sliceWidth,
      sliceStartIdx,
      a_rows = a->getNumRows(),
      a_cols = a->getNumCols(),
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
  this->numSlices = size;

  if (this->numSlices > a_rows){
    fprintf(stderr, "WARNING:\n");
    fprintf(stderr, "Number of slices must be less than or equal to number of rows in A. Program will set numSlices to the number of columns of A.");
    this->numSlices = a_rows;
  }
 
  // ensure our numSlices evenly divides our number of rows of A
  if (a_rows % this->numSlices){
    if (rank==0){
      fprintf(stderr, "ERROR:\n");
      fprintf(stderr, "The algorithm only supports slices sizes that evenly divide the number of rows of A.\n");
    }
    exit(1);
  }
  sliceWidth = ceil((double)(a_rows/this->numSlices));


  dataManager.startTimer(); // MPI begins here

  // the row and index of our resulting matrix C
  sliceStartIdx = rank*sliceWidth*a_cols;

  // pointers starting at our needed offsets
  localA = &a_raw[sliceStartIdx];
  localB = b_raw;
  localC = &c_raw[rank*sliceWidth*c_cols];

  #pragma omp parallel for num_threads(this->numThreads)
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
  int rowIdx          = rank*sliceWidth,
      startIdx        = rowIdx*c->getNumCols(),
      receiveIdx;
  
  MPI_Status status;
  MPI_Datatype sliceType;
  MPI_Type_vector(sliceWidth, c->getNumCols(), c->getNumCols(), MPI_DOUBLE, &sliceType);
  MPI_Type_commit(&sliceType);

  if (rank>0)
    MPI_Send(&c_raw[startIdx], 1, sliceType, 0, 0, MPI_COMM_WORLD);

  if (rank==0){
    for (int sendingRank=1; sendingRank<this->numSlices; ++sendingRank){
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

  #pragma omp parallel for num_threads(this->numThreads*this->numSlices)
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



double MatrixMultiplication::getTimeElapsedOptimal(){
  return this->timeElapsedOptimal;
}



double MatrixMultiplication::getTimeElapsedSubOptimal(){
  return this->timeElapsedSubOptimal;
}