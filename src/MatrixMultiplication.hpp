#include <sys/time.h>

#define ULIMIT 10
#define OUT_FILE1 "./algorithm1.log"
#define OUT_FILE2 "./algorithm2.log"
#define OUT_FILE3 "./algorithm3.log"

class MatrixMultiplication{
  private:
    Matrix* a;
    Matrix* b;
    Matrix* c;
  public:
    MatrixMultiplication(Matrix* aInput, Matrix* bInput);
    ~MatrixMultiplication();
    void reinitializeC();
    Matrix* algorithm1();
    Matrix* algorithm2();
    Matrix* algorithm3();
};
