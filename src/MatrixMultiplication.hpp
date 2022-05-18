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
     *              DataManager associated with this algorithm.
     *
     * @return A matrix C which is the product of the objects fields A and B.
     */
    Matrix* multiply();

    /**
     * Algorithm for matrix multiplication using ikj indexing. This uses both MPI and OpenMP
     * to run in parallel. It should be very fast.
     * Side effect: Calculates the elapsed time (in seconds) and records it to a
     *              DataManager associated with this algorithm.
     *
     * @return A matrix C which is the product of the objects fields A and B.
     */
    Matrix* multiply_parallel();
    
    /**
     * Used to gather all the data from all MPI processes into a single rank's C buffer.
     *
     * @param rank - the rank of the MPI process
     * @param numSlices - the number of processes in this MPI program
     * @param c - this rank's contribution towards the product
     */
    void consolidateC(int rank, int sliceWidth, int numSlices, double* c);
    
    /**
     * Get the rank of the MPI process running this object.
     *
     * @return The rank of the MPI process.
     */
    int getRank(){ return myRank; }

    /**
     * Used to Get the resulting matrix C.
     *
     * @return The resulting matrix C.
     */
    Matrix* getResult(){ return c; }

    /**
     * Used to tell the algorithm how many slices and threads to use.
     *
     * @param numThreads - The number of threads PER MPI process
     * @param numSlices - The number of slices (number of MPI processes)
     */
    void setThreadSlices(int numThreads, int numSlices);

    /**
     * Get the time elapsed of the optimal algorithm.
     *
     * @return The time elapsed of the OpenMP/MPI algorithm.
     */
    double getTimeElapsedOptimal();

    /**
     * Get the time elapsed of the suboptimal algorithm.
     *
     * @return The time elapsed of the pure OpenMP algorithm.
     */
    
    double getTimeElapsedSubOptimal();
};

#endif // MATRIX_MULTIPLICATION_HPP
