#include "Matrix.hpp"

Matrix::Matrix(int m, int n):rows(m), cols(n){
  matrix = new double*[m];
  for (int i=0; i<m; ++i){
    matrix[i] = new double[n];
  }
}


Matrix::~Matrix(){
  for (int i=0; i<rows; ++i){
    delete[] matrix[i];
  }
  delete[] matrix;
}


void Matrix::randomize(int limit){
  srand(time(NULL));
  for (int i=0; i<rows; ++i){
    for (int j=0; j<cols; ++j){
      matrix[i][j] = limit * ((double)rand() / (double)limit);
    }
  }
}


void Matrix::reinitialize(){
  randomize(1);
}


void Matrix::printMatrix(){
  for (int i=0; i<rows; ++i){
    for (int j=0; j<cols; ++j){
      std::cout << matrix[i][j] << DELIMETER;
    }
    std::cout << NEWLINE;
  }
}


double** Matrix::getMatrix(){
  return this->matrix;
}

int Matrix::getNumRows(){
  return this->rows;
}

int Matrix::getNumCols(){
  return this->cols;
}
