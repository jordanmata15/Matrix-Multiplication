#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "iostream"

// For ease in changing how to print the array
#define DELIMETER " "
#define NEWLINE "\n"

/**
 * Represent a matrix with 2D integer arrays.
 */
class Matrix{
  private:
    int rows;
    int cols;
    double** matrix;

  public:
    /**
     * Dynamically allocates memory for our 2D matrix to allow for very large
     * matrices that would otherwise not fit on the stack.
     *
     * @param m Number of rows
     * @param n Number of columns
     */
    Matrix(int m, int n);
    
    /**
     * Releases the memory from our dynamic allocation.
     */
    ~Matrix();
    
    /**
     * Accessor for the number or rows in the matrix.
     */
    int getNumRows();
    
    /**
     * Accessor for the number of columns in the matrix.
     */
    int getNumCols();
    
    /**
     * Reinitializes the matrix values by assigning them a value between 0 and
     * the desired limit.
     *
     * @param limit Maximum value that can be held by each cell of the matrix.
     */
    void randomize(int limit);
    
    /**
     * Reinitializes the matrix values by assigning all values to 0.
     */
    void reinitialize();
    
    /**
     * Accessor method for returning a reference to the underlying matrix 
     * itself. This is poor OOP form, but we want to avoid the overhead of 
     * calling something like get(X,Y) to get more accurate benchmarks.
     *
     * @return the raw 2D matrix.
     */
    double** getMatrix();
    
    /**
     * Prints the values of the matrix with each item being delimited by
     * DELIMETER.
     */
    void printMatrix();
};

#endif // MATRIX_HPP
