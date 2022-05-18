#include "main.hpp"

int main(int argc, char** argv){
  ArgParser ap = ArgParser();
  Arguments* args = ap.parseArgs(argc, argv);

  // initialize matrices
  Matrix* a = new Matrix(args->rowsA, args->colsA);
  a->randomize(ULIMIT);
  Matrix* b = new Matrix(args->rowsB, args->colsB);
  b->randomize(ULIMIT);

  // Object that we can call to do each of the multiplication algorithms for us
  MatrixMultiplication matrixMult = MatrixMultiplication(a, b); 

  // tell the matrixMult class how many threads we are using
  matrixMult.setThreads(args->numThreads);
  
  // perform the matrix multiplication
  matrixMult.multiply_parallel();
  
  if (matrixMult.getRank() != 0)
    return 0;
  else {
    // print out NUM_THREADS,NUM_SLICES,TIME_ELAPSED
    std::cout << std::fixed << std::setprecision(10) << matrixMult.getTimeElapsedOptimal() << std::endl;
  }

  delete a;
  delete b;
  return 0;
}
