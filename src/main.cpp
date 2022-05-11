#include "main.hpp"

int main(int argc, char** argv){
  ArgParser ap = ArgParser();
  Arguments* args = ap.parseArgs(argc, argv);

  // initialize matrices
  Matrix* a = new Matrix(args->rowsA, args->colsA);
  a->randomize(ULIMIT);
  Matrix* b = new Matrix(args->rowsB, args->colsB);
  b->randomize(ULIMIT);


  // Data manager for benchmarking time
  DataManager dataManager = DataManager();

  // Object that we can call to do each of the multiplication algorithms for us
  MatrixMultiplication matrixMult = MatrixMultiplication(a, b); 


  Matrix  * product,
          * copy;

  product = matrixMult.multiply_parallel();
  matrixMult.setThreadTiles(args->numThreads, args->numTiles);

  if (matrixMult.getRank() != 0) return 0;

  int verifyResult = 1;

  if (verifyResult){
    copy = new Matrix(*product);
    matrixMult.reinitializeC();

    if (!matrixMult.verify(copy)){
      fprintf(stderr, "The matrix multiplication failed!!!\n");
      std::fflush;
      std::cout << "\nOutput calculated:" << std::endl;
      copy->printMatrix('\t');
      std::cout << "\nOutput expected:" << std::endl;
      matrixMult.getResult()->printMatrix('\t\t');
      return -1;
    }
    else {
      std::cout << "Matrix multiplication succeeded!" << std::endl;
    }
  }

  if (0){
    product->printMatrix();
    //std::cout << std::fixed << std::setprecision(10) << dataManager.getTimeElapsed() << std::endl;
  }

  delete a;
  delete b;
  return 0;
}