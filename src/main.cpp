  #include "main.hpp"

int main(int argc, char** argv){
  ArgParser ap = ArgParser();
  Arguments* args = ap.parseArgs(argc, argv);

  // initialize matrices
  Matrix* a = new Matrix(args->rowsA, args->colsA);
  a->randomize(ULIMIT);
  Matrix* b = new Matrix(args->rowsB, args->colsB);
  b->randomize(ULIMIT);

  if (args->displayAB){
    std::cout << NEW_SECTION << "\tMatrices A and B\t" << NEW_SECTION;
    std::cout << "Matrix A:\n";
    a->printMatrix();
    std::cout << "Matrix B:\n";
    b->printMatrix();
  }

  // Data manager for benchmarking time
  DataManager dataManager = DataManager();

  // Object that we can call to do each of the multiplication algorithms for us
  MatrixMultiplication matrixMult = MatrixMultiplication(a, b, &dataManager); 

  Matrix* product;
  if      (args->algNum==0) product = matrixMult.algorithm0();
  else if (args->algNum==1) product = matrixMult.algorithm1();
  else                      product = matrixMult.algorithm2();

  if (args->displayC) product->printMatrix();
  
  std::cout << std::fixed << std::setprecision(10) << dataManager.average() << std::endl;

  delete a;
  delete b;
  return 0;
}