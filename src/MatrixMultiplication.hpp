#ifndef MATRIX_MULTIPLICATION_HPP
#define MATRIX_MULTIPLICATION_HPP

#include <math.h>
#include <mpi.h>
#include <omp.h>
#include <sys/time.h>

#include "ArgParser.hpp"
#include "Matrix.hpp"
#include "DataManager.hpp"

#define ULIMIT 10   // largest value for each entry in a matrix
#define NUM_ALGORITHMS 3
#define NEW_SECTION "\n*******************************************\n"

/**
 * Class used to represent the matrix multiplication operation. Stores two 
 * matrices (A,B) and allows calls on different multiplication algorithms
 * for benchmarking.
 */
class MatrixMultiplication{

  private:
    Matrix* a;
    Matrix* b;
    Matrix* c;
    DataManager dataManager; // containers to hold the time benchmarks.
    int numSlices;
    int numThreads;
    int myRank;
    double timeElapsedOptimal;
    double timeElapsedSubOptimal;

  public:
    /**
     * MatrixMultiplication constructor. Initializes space for the C matrix, as 
     * it may be large.
     *
     * @param aInput The matrix A to act as the left operand.
     * @param bInput The matrix B to act as teh right operand.
     * @param dm Data storage for easy recording/manipulation of our times.
     */
    MatrixMultiplication(Matrix* aInput, 
                          Matrix* bInput, 
                          DataManager dm);

    MatrixMultiplication(Matrix* aInput, 
                          Matrix* bInput);

    /**
     * MatrixMultiplication Destructor that frees matrix C.
     */
    ~MatrixMultiplication();
    
    /**
     * Zeroes out the C matrix. Useful as some algorithms depend on C being
     * 0-filled.
     */
    void reinitializeC();
    
    /**
     * Algorithm for matrix multiplication using ikj indexing. This uses
     * row accesses. It should be relatively fast.
     * Side effect: Calculates the elapsed time (in seconds) and records it to a
     *              DataManager associated with this algorithm (once for each 
     *              time this function is called).
     *
     * @return A matrix C which is the product of the objects fields A and B.
     */
    Matrix* multiply();

    Matrix* multiply_parallel();
    void rotate(int rank, int tileDim, int tilesPerDim, double* a, double* b, int rowsA, int rowsB, int maxDim);
    int getStartIndex(int rank, int numCols, int tileDim);
    void consolidateC(int rank, int sliceWidth, int numSlices, double* c);
    bool verify(Matrix* m);

    int getRank(){ return myRank; }

    Matrix* getResult(){ return c; }
    void setThreadTiles(int numThreads, int numTiles);

    double getTimeElapsedOptimal();

    double getTimeElapsedSubOptimal();
};

#endif // MATRIX_MULTIPLICATION_HPP
