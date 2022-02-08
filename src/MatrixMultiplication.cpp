#include "MatrixMultiplication.hpp"


MatrixMultiplication::MatrixMultiplication(Matrix* aInput, 
                                           Matrix* bInput, 
                                           std::vector<DataManager>* dm){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
  dataManagers = dm;
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
  dataManagers->at(0).recordTime(&elapsedTime);
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
  dataManagers->at(1).recordTime(&elapsedTime);
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
  dataManagers->at(2).recordTime(&elapsedTime);
  return c;
}


int main(int argc, char** argv){
  ArgParser ap = ArgParser();
  Arguments* args = ap.parseArgs();

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
  
  /* initialized data managers for benchmarking time (one for each algorithm)
   * files written to are "algorithm#.log" in the calling directory */
  std::vector<DataManager> dataManagers = std::vector<DataManager>();
  for (int i=0; i<NUM_ALGORITHMS; ++i){
    std::string fileName = "./algorithm" + std::to_string(i) + ".log";
    dataManagers.insert(dataManagers.begin()+i, DataManager(fileName));
  }

  // Object that we can call to do each of the multiplication algorithms for us
  MatrixMultiplication matrixMult = MatrixMultiplication(a, b, &dataManagers); 


  std::cout << NEW_SECTION << "\tRunning matrix multiplication\t" << NEW_SECTION;
  /* Get multiple runs of each algorithm for a good average. Reinitializing A/B 
   * to a new matrix each time is pointless since we don't actually care about 
   * their values, just time benchmarks. */
  for(int i=0; i<args->numRuns; ++i){
  Matrix* product;

    for (int algNum=0; algNum<NUM_ALGORITHMS; ++algNum){ // run all 3 algorithms
      matrixMult.reinitializeC();
      if      (algNum==0) product = matrixMult.algorithm0();
      else if (algNum==1) product = matrixMult.algorithm1();
      else if (algNum==2) product = matrixMult.algorithm2();
      // only print C on the first iteration (all others are the same)
      if (args->displayC && i==0){
        std::cout << NEW_SECTION << "\tMatrix C\t" << NEW_SECTION;
        std::cout << "\nProduct using algorithm " << algNum << ": " << std::endl;
        product->printMatrix();
      }
    }
  }
  
  if (args->displayAverages){
    std::cout << NEW_SECTION << "\tAVERAGES\t" << NEW_SECTION;
    std::cout << "Avg runtimes (in sec) over " << args->numRuns << " iterations:\n";
    int i=0;
    for (auto dm:dataManagers){
      std::cout << "Algorithm " << i << ": " << dm.average() << std::endl;
      ++i;
    }
  }

  delete a;
  delete b;

  return 0;
}
