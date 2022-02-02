#include "ArgParser.hpp"
#include "MatrixGenerator.hpp"

int main(int argc, char** argv){
  ArgParser ap = ArgParser();
  Arguments* args = ap.parseArgs(argc, argv);

  MatrixGenerator mg = MatrixGenerator(args->rows, args->cols);
  
  int** A = mg.getMatrix();
  std::cout << "Matrix A:\n";
  mg.printMatrix(A);
  mg.refresh();
  
  int** B = mg.getMatrix();
  std::cout << "Matrix B:\n";
  mg.printMatrix(B);
  return 0;
}
