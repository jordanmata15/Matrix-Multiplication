#include "ArgParser.hpp"
#include "Matrix.hpp"
#include "DataManager.hpp"

#include <sys/time.h>

#define ULIMIT 10
#define NUM_ITERS 3

class MatrixMultiplication{
  private:
    Matrix* a;
    Matrix* b;
    Matrix* c;
    DataManager* dataMgr;
  public:
    MatrixMultiplication(Matrix* aInput, Matrix* bInput, DataManager* dm);
    ~MatrixMultiplication();
    void reinitializeC();
    Matrix* algorithm1();
    Matrix* algorithm2();
    Matrix* algorithm3();
};
