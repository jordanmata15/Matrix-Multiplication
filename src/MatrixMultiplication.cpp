#include "MatrixMultiplication.hpp"


MatrixMultiplication::MatrixMultiplication(Matrix* aInput, 
                                           Matrix* bInput, 
                                           DataManager* dm,
                                           int numThreads){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
  dataManager = dm;
  this->numThreads = numThreads;
}


MatrixMultiplication::~MatrixMultiplication(){
  delete c;
}


void MatrixMultiplication::reinitializeC(){
  c->reinitialize(); // zero fill C
}

// leverages row major. Very good for C++.
Matrix* MatrixMultiplication::multiply(){
  int a_rows = a->getNumRows(),
      a_cols = a->getNumCols(),
      b_rows = b->getNumRows(),
      b_cols = b->getNumCols(),
      c_rows = c->getNumRows();
  
  /* same raw access as algorithm 0 */
  double* a_raw = a->getMatrix();
  double* b_raw = b->getMatrix();
  double* c_raw = c->getMatrix();
  
  dataManager->startTimer();

  //#pragma omp parallel for num_threads(this->numThreads)
  for (int i=0; i<a_rows; ++i){
    for (int k=0; k<a_cols; ++k){
      double r = a_raw[i+a_rows*k];
      for (int j=0; j<b_cols; ++j){
        c_raw[i+c_rows*j] += r * b_raw[k+b_rows*j];
      }
    }
  }

  dataManager->stopTimer();
  return c;
}