#include "ArgParser.hpp"
#include "Matrix.hpp"
#include "MatrixMultiplication.hpp"

#define DEBUG

MatrixMultiplication::MatrixMultiplication(Matrix* aInput, Matrix* bInput){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
}

MatrixMultiplication::~MatrixMultiplication(){
  free(c);
}


void MatrixMultiplication::reinitializeC(){
  c->reinitialize();
}

// leverages column major (needs to be zero filled beforehand)
Matrix* MatrixMultiplication::algorithm1(){
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();

  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int i=0; i<a_rows; i++){
    for (int j=0; j<b_cols; j++){
      for (int k=0; k<a_cols; k++){
        c_raw[i][j] += a_raw[i][k] * b_raw[k][j];
      }
    }
  }
  gettimeofday(&endTime, NULL);
  
  timersub(&endTime, &startTime, &elapsedTime);
  long elapsedSeconds = elapsedTime.tv_sec + (elapsedTime.tv_usec/1000000);
  
  

  #ifdef DEBUG
  std::cout << "Time Elapsed (in seconds): " << elapsedSeconds << std::endl;
  #endif

  return c;
}


// leverages column major (needs to be zero filled beforehand)
Matrix* MatrixMultiplication::algorithm2(){
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();

  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int j=0; j<b_cols; j++){
    for (int k=0; k<a_cols; k++){
      int r = b_raw[k][j];
      for (int i=0; i<a_rows; i++){
        c_raw[i][j] += a_raw[i][k] * r;
      }
    }
  }
  gettimeofday(&endTime, NULL);
  
  timersub(&endTime, &startTime, &elapsedTime);
  long elapsedSeconds = elapsedTime.tv_sec + (elapsedTime.tv_usec/1000000);
  
  #ifdef DEBUG
  std::cout << "Time Elapsed (in seconds): " << elapsedSeconds << std::endl;
  #endif
  
  return c;
}

// leverages row major (needs to be zero filled beforehand)
Matrix* MatrixMultiplication::algorithm3(){
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();
  
  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int i=0; i<a_rows; i++){
    for (int k=0; k<a_cols; k++){
      int r = a_raw[i][k];
      for (int j=0; j<b_cols; j++){
        c_raw[i][j] += r * b_raw[k][j];
      }
    }
  }
  gettimeofday(&endTime, NULL);
  
  timersub(&endTime, &startTime, &elapsedTime);
  long elapsedSeconds = elapsedTime.tv_sec + (elapsedTime.tv_usec/1000000);
  
  #ifdef DEBUG
  std::cout << "Time Elapsed (in seconds): " << elapsedSeconds << std::endl;
  #endif
  
  return c;
}


int main(int argc, char** argv){
  ArgParser ap = ArgParser();
  Arguments* args = ap.parseArgs();

  Matrix* a = new Matrix(args->rowsA, args->colsA);
  a->randomize(ULIMIT);
  Matrix* b = new Matrix(args->rowsB, args->colsB);
  b->randomize(ULIMIT);
  
  #ifdef DEBUG
  std::cout << "Matrix A:\n";
  a->printMatrix();
  std::cout << "Matrix B:\n";
  b->printMatrix();
  #endif

  MatrixMultiplication mm = MatrixMultiplication(a, b);
  std::cout << "Product using algorithm 1:" << std::endl;
  mm.algorithm1()->printMatrix();
  
  // we reuse matrix C, zero it out as some algorithms expect it to be 0 filled
  mm.reinitializeC();
  std::cout << "\nProduct using algorithm 2:\n";
  mm.algorithm2()->printMatrix();

  mm.reinitializeC();
  std::cout << "\nProduct using algorithm 3:\n";
  mm.algorithm3()->printMatrix();
  
  return 0;
}
