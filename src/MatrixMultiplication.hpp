#ifndef MATRIX_MULTIPLICATION_HPP
#define MATRIX_MULTIPLICATION_HPP

#include "ArgParser.hpp"
#include "Matrix.hpp"
#include "DataManager.hpp"
#include <sys/time.h>

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
    std::vector<DataManager>* dataManagers; // containers to hold the time 
                                            // benchmarks. 1 for each algorithm

  public:
    /**
     * MatrixMultiplication constructor. Initializes space for the C matrix, as 
     * it may be large.
     *
     * @param aInput The matrix A to act as the left operand.
     * @param bInput The matrix B to act as teh right operand.
     * @param dm The vector of data storages for easy recording/manipulation of 
     *           our times.
     */
    MatrixMultiplication(Matrix* aInput, 
                          Matrix* bInput, 
                          std::vector<DataManager>* dm);

    /**
     * MAtrixMultiplication Destructor that frees matrix C.
     */
    ~MatrixMultiplication();
    
    /**
     * Zeroes out the C matrix. Useful as some algorithms depend on C being
     * 0-filled.
     */
    void reinitializeC();

    /**
     * Algorithm for matrix multiplication using ijk indexing. This uses both
     * column/row accesses. It should be relatively average.
     * Side effect: Calculates the elapsed time (in seconds) and records it to a
     *              DataManager associated with this algorithm (once for each 
     *              time this function is called).
     *
     * @return A matrix C which is the product of the objects fields A and B.
     */
    Matrix* algorithm0();
    
    /**
     * Algorithm for matrix multiplication using jki indexing. This uses
     * column accesses. It should be relatively slow.
     * Side effect: Calculates the elapsed time (in seconds) and records it to a
     *              DataManager associated with this algorithm (once for each 
     *              time this function is called).
     *
     * @return A matrix C which is the product of the objects fields A and B.
     */
    Matrix* algorithm1();
    
    /**
     * Algorithm for matrix multiplication using ikj indexing. This uses
     * row accesses. It should be relatively fast.
     * Side effect: Calculates the elapsed time (in seconds) and records it to a
     *              DataManager associated with this algorithm (once for each 
     *              time this function is called).
     *
     * @return A matrix C which is the product of the objects fields A and B.
     */
    Matrix* algorithm2();
};

#endif // MATRIX_MULTIPLICATION_HPP
