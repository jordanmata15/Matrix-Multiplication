#include "MatrixGenerator.hpp"
#include "iostream" // TODO

MatrixGenerator::MatrixGenerator(int m, int n): rows(m), cols(n){
  std::cout << "Creatingn an array with rows/cols = " << m << n <<std::endl;
  matrix = new int*[m];
  for (int i=0; i<m; i++){
    matrix[i] = new int[n];
  }
  refresh();
}

void MatrixGenerator::refresh(){
  for (int i=0; i<rows; i++){
    for (int j=0; j<cols; j++){
      matrix[i][j] = rand() % ULIMIT;
    }
  }
}

void MatrixGenerator::printMatrix(int** matrix){
  for (int i=0; i<rows; i++){
    for (int j=0; j<cols; j++){
      std::cout << matrix[i][j];
    }
    std::cout << "\n";
  }
}


int** MatrixGenerator::getMatrix(){
  return this->matrix;
}
