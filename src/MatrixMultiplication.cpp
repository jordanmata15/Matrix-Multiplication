#include "MatrixMultiplication.hpp"


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
  c->reinitialize(); // zero fill C
}


// Uses both row/column major operations. Best/worst of both.
Matrix* MatrixMultiplication::algorithm0(){
  
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();

  /* We could define matrix->getXY(x,y) which would be better from an OOP 
   * perspective, but direct access to the raw array memory is faster as it 
   * avoids the overhead of a function call for EACH access.
   */
  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int i=0; i<a_rows; ++i){
    for (int j=0; j<b_cols; ++j){
      int sum = 0;
      for (int k=0; k<a_cols; ++k){
        sum += a_raw[i][k] * b_raw[k][j];
      }
      c_raw[i][j] = sum;
    }
  }
  gettimeofday(&endTime, NULL);

  timersub(&endTime, &startTime, &elapsedTime);
  dataMgr->recordTime(0, &elapsedTime);
  return c;
}


// leverages column major. Very Bad for C++.
Matrix* MatrixMultiplication::algorithm1(){
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();

  /* same raw access as algorithm 0 */
  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int j=0; j<b_cols; ++j){
    for (int k=0; k<a_cols; ++k){
      int r = b_raw[k][j];
      for (int i=0; i<a_rows; ++i){
        c_raw[i][j] += a_raw[i][k] * r;
      }
    }
  }
  gettimeofday(&endTime, NULL);
  
  timersub(&endTime, &startTime, &elapsedTime);
  dataMgr->recordTime(1, &elapsedTime);
  return c;
}


// leverages row major. Very good for C++.
Matrix* MatrixMultiplication::algorithm2(){
  int a_rows = a->getNumRows();
  int a_cols = a->getNumCols(); // equal to b_rows
  int b_cols = b->getNumCols();
  
  /* same raw access as algorithm 0 */
  int** a_raw = a->getMatrix();
  int** b_raw = b->getMatrix();
  int** c_raw = c->getMatrix();
  
  struct timeval startTime, endTime, elapsedTime;
  gettimeofday(&startTime, NULL);
  for (int i=0; i<a_rows; ++i){
    for (int k=0; k<a_cols; ++k){
      int r = a_raw[i][k];
      for (int j=0; j<b_cols; ++j){
        c_raw[i][j] += r * b_raw[k][j];
      }
    }
  }
  gettimeofday(&endTime, NULL);
  
  timersub(&endTime, &startTime, &elapsedTime);
  dataMgr->recordTime(2, &elapsedTime);
  return c;
}


int main(int argc, char** argv){
  ArgParser ap = ArgParser();
  Arguments* args = ap.parseArgs();

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

  std::vector<std::string> fileNames = {OUT_FILE1, OUT_FILE2, OUT_FILE3};
  DataManager* dm = new DataManager(fileNames);
  MatrixMultiplication mm = MatrixMultiplication(a, b, dm); 
  Matrix* product;

  std::cout << NEW_SECTION << "\tRunning matrix multiplication\t" << NEW_SECTION;

  /* Get multiple runs of each algorithm for a good average. Randomizing values 
   * in A/B each time is pointless since we don't actually care about their 
   * values, just time benchmarks.
   */
  for(int i=0; i<args->numRuns; ++i){
    
    // algorithm 0
    mm.reinitializeC();
    product = mm.algorithm0();
    if (args->displayC){
      if (i==0){
        std::cout << NEW_SECTION << "\tMatrix C\t" << NEW_SECTION;
        std::cout << "\nProduct using algorithm 0:" << std::endl;
        product->printMatrix();
      }
    }
    
    // algorithm 1
    mm.reinitializeC();
    product = mm.algorithm1();
    if (args->displayC){
      if (i==0){
        std::cout << "\nProduct using algorithm 1:" << std::endl;
        product->printMatrix();
      }
    }

    // algorithm 2
    mm.reinitializeC();
    product = mm.algorithm2();
    if (args->displayC){
      if (i==0){
        std::cout << "\nProduct using algorithm 2:" << std::endl;
        product->printMatrix();
      }
    }
  }
  
  if (args->displayAverages){
    std::cout << NEW_SECTION << "\tAVERAGES\t" << NEW_SECTION;
    std::cout << "Avg runtimes (in sec) over " << args->numRuns << " iterations:\n";
    for (int i=0; i<NUM_ALGORITHMS; ++i){
      double avgRuntimeSeconds = dm->takeAverageOfAlg(i);
      std::cout << "Algorithm " << i << ": " << avgRuntimeSeconds << std::endl;
    }
  }

  delete a;
  delete b;
  delete dm;

  return 0;
}
