#include "MatrixMultiplication.hpp"


MatrixMultiplication::MatrixMultiplication(Matrix* aInput, 
                                           Matrix* bInput, 
                                           DataManager* dm){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
  dataManagers = dm;
}


MatrixMultiplication::~MatrixMultiplication(){
  delete c;
}


void MatrixMultiplication::reinitializeC(){
  c->reinitialize(); // zero fill C
}


// Uses both row/column major operations. Best/worst of both.
Matrix* MatrixMultiplication::algorithm0(){
  
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();

  /* We could define matrix->getXY(x,y) which would be better from an OOP 
   * perspective, but direct access to the raw array memory is faster as it 
   * avoids the overhead of a function call for EACH access.
   */
  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int i=0; i<a_rows; ++i){
    for (int j=0; j<b_cols; ++j){
      int sum = 0;
      for (int k=0; k<a_cols; ++k){
        sum += a_raw[i][k] * b_raw[k][j];
      }
      c_raw[i][j] = sum;
    }
  }
  gettimeofday(&endTime, NULL);

  timersub(&endTime, &startTime, &elapsedTime);
  dataManagers->recordTime(&elapsedTime);
  return c;
}


// leverages column major. Very Bad for C++.
Matrix* MatrixMultiplication::algorithm1(){
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();

  /* same raw access as algorithm 0 */
  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int j=0; j<b_cols; ++j){
    for (int k=0; k<a_cols; ++k){
      int r = b_raw[k][j];
      for (int i=0; i<a_rows; ++i){
        c_raw[i][j] += a_raw[i][k] * r;
      }
    }
  }
  gettimeofday(&endTime, NULL);
  
  timersub(&endTime, &startTime, &elapsedTime);
  dataManagers->recordTime(&elapsedTime);
  return c;
}


// leverages row major. Very good for C++.
Matrix* MatrixMultiplication::algorithm2(){
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();
  
  /* same raw access as algorithm 0 */
  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int i=0; i<a_rows; ++i){
    for (int k=0; k<a_cols; ++k){
      int r = a_raw[i][k];
      for (int j=0; j<b_cols; ++j){
        c_raw[i][j] += r * b_raw[k][j];
      }
    }
  }
  gettimeofday(&endTime, NULL);
  
  timersub(&endTime, &startTime, &elapsedTime);
  dataManagers->recordTime(&elapsedTime);
  return c;
}