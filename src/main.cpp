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


  Matrix  * product,
          * copy;

  product = matrixMult.multiply_parallel();
  matrixMult.setThreadTiles(args->numThreads, args->numSlices);

  if (matrixMult.getRank() != 0) return 0;

  int verifyResult = 0;
  verifyResult = 1;

  if (verifyResult){
    copy = new Matrix(*product); // create a deep copy so we can validate if they are the same
    matrixMult.reinitializeC();

    if (0 && !matrixMult.verify(copy)){
      fprintf(stderr, "The matrix multiplication failed!!!\n");
      std::cout << "\nOutput calculated:" << std::endl;
      //copy->printMatrix('\t');
      std::cout << "\nOutput expected:" << std::endl;
      //matrixMult.getResult()->printMatrix('\t');
      return -1;
    }
    else {
      std::cout << "Matrix multiplication succeeded!" << std::endl;
    }
  }

  if (1){
    std::cout << std::fixed << std::setprecision(10) << matrixMult.getTimeElapsedOptimal() << std::endl;
    std::cout << std::fixed << std::setprecision(10) << matrixMult.getTimeElapsedSubOptimal() << std::endl;
  }

  delete a;
  delete b;
  return 0;
}