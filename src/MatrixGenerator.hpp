#ifndef MATRIX_GENERATOR_H
#define MATRIX_GENERATOR_H

#define ULIMIT 10

class MatrixGenerator {
  private:
    const int rows;
    const int cols;
    int** matrix;
  public:
    MatrixGenerator(int m, int n);
    void refresh();
    int** getMatrix();
    void printMatrix(int** matrix);
};

#endif // 
