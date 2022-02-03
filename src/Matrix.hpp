#ifndef MATRIX_H
#define MATRIX__H

#define DELIMETER " "
#define NEWLINE "\n"

class Matrix{
  public:
    int rows;
    int cols;
    int** matrix;
    Matrix(int m, int n);
    ~Matrix();
    void randomize(int limit);
    void reinitialize();
    int** getMatrix();
    int getNumRows();
    int getNumCols();
    void printMatrix();
};

#endif // MATRIX_H
