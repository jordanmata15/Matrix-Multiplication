#include "MatrixMultiplication.hpp"

// #define DEBUG
// #define VERBOSE
#define AVERAGES

MatrixMultiplication::MatrixMultiplication(Matrix* aInput, 
                                            Matrix* bInput, 
                                            DataManager* dm){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
  dataMgr = dm;
}

MatrixMultiplication::~MatrixMultiplication(){
  delete c;
}


void MatrixMultiplication::reinitializeC(){
  c->reinitialize();
}

// leverages both majors. Best/worst of both. (needs to be zero filled beforehand)
Matrix* MatrixMultiplication::algorithm1(){
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();

  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int i=0; i<a_rows; i++){
    for (int j=0; j<b_cols; j++){
      for (int k=0; k<a_cols; k++){
        c_raw[i][j] += a_raw[i][k] * b_raw[k][j];
      }
    }
  }
  gettimeofday(&endTime, NULL);

  timersub(&endTime, &startTime, &elapsedTime);
  dataMgr->recordTime(1, &elapsedTime);
  return c;
}


// leverages column major (needs to be zero filled beforehand)
Matrix* MatrixMultiplication::algorithm2(){
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();

  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int j=0; j<b_cols; j++){
    for (int k=0; k<a_cols; k++){
      int r = b_raw[k][j];
      for (int i=0; i<a_rows; i++){
        c_raw[i][j] += a_raw[i][k] * r;
      }
    }
  }
  gettimeofday(&endTime, NULL);
  
  timersub(&endTime, &startTime, &elapsedTime);
  dataMgr->recordTime(2, &elapsedTime);
  return c;
}

// leverages row major (needs to be zero filled beforehand)
Matrix* MatrixMultiplication::algorithm3(){
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();
  
  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int i=0; i<a_rows; i++){
    for (int k=0; k<a_cols; k++){
      int r = a_raw[i][k];
      for (int j=0; j<b_cols; j++){
        c_raw[i][j] += r * b_raw[k][j];
      }
    }
  }
  gettimeofday(&endTime, NULL);
  
  timersub(&endTime, &startTime, &elapsedTime);
  dataMgr->recordTime(3, &elapsedTime);
  return c;
}


int main(int argc, char** argv){
  ArgParser ap = ArgParser();
  Arguments* args = ap.parseArgs();

  Matrix* a = new Matrix(args->rowsA, args->colsA);
  a->randomize(ULIMIT);
  Matrix* b = new Matrix(args->rowsB, args->colsB);
  b->randomize(ULIMIT);
  
  #ifdef DEBUG
  std::cout << "Matrix A:\n";
  a->printMatrix();
  std::cout << "Matrix B:\n";
  b->printMatrix();
  #endif

  std::vector<std::string> fileNames = {OUT_FILE1, OUT_FILE2, OUT_FILE3};
  DataManager* dm = new DataManager(fileNames);
  MatrixMultiplication mm = MatrixMultiplication(a, b, dm); 
  Matrix* product;

  for(int i=0; i<NUM_ITERS; ++i){
    
    mm.reinitializeC();
    product = mm.algorithm1();
    #ifdef VERBOSE
    std::cout << "Product using algorithm 1:" << std::endl;
    product->printMatrix();
    #endif 
    
    mm.reinitializeC();
    product = mm.algorithm2();
    #ifdef VERBOSE
    std::cout << "\nProduct using algorithm 2:\n";
    product->printMatrix();
    #endif 

    mm.reinitializeC();
    product = mm.algorithm3();
    #ifdef VERBOSE
    std::cout << "\nProduct using algorithm 3:\n";
    product->printMatrix();
    #endif 
  }
  
  #ifdef AVERAGES
  std::cout << "Average runtimes (in seconds) over " << NUM_ITERS << " iterations:\n";
  for (int i=1; i<=3; ++i){
    double avgRuntimeSeconds = dm->takeAverageOfAlg(i);
    std::cout << "Algorithm " << i << ": " << avgRuntimeSeconds << std::endl;
  }
  #endif

  delete a;
  delete b;
  delete dm;

  return 0;
}
