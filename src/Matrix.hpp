#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "iostream"

#define DELIMETER " "
#define NEWLINE "\n"

class Matrix{
  private:
    int rows;
    int cols;
    int** matrix;

  public:
    Matrix(int m, int n);
    ~Matrix();
    void randomize(int limit);
    void reinitialize();
    int** getMatrix();
    int getNumRows();
    int getNumCols();
    void printMatrix();
};

#endif // MATRIX_HPP
