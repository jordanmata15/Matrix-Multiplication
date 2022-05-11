#include "MatrixMultiplication.hpp"

#include <unistd.h>
#include <cstdlib>


MatrixMultiplication::MatrixMultiplication(Matrix* aInput, 
                                           Matrix* bInput, 
                                           DataManager dm){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
  dataManager = dm;
  this->numThreads = numThreads;
}

MatrixMultiplication::MatrixMultiplication(Matrix* aInput, 
                                           Matrix* bInput){
  a = aInput;
  b = bInput;
  c = new Matrix(a->getNumRows(), b->getNumCols());
  dataManager = DataManager();
  this->numThreads = numThreads;
}

void MatrixMultiplication::setThreadTiles(int numThreads, int numTiles){
  this->numThreads = numThreads;
  this->numTiles = numTiles;
}


MatrixMultiplication::~MatrixMultiplication(){
  delete c;
}


void MatrixMultiplication::reinitializeC(){
  c->reinitialize(); // zero fill C
}

// leverages row major. Very good for C++.
Matrix* MatrixMultiplication::multiply_parallel(){
  //this->numThreads = numThreads;
  //this->numTiles = numTiles;
  int size, rank;

  MPI_Init(nullptr, nullptr);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  this->myRank = rank;
  this->numTiles = size;
  
  int a_rows    = a->getNumRows(),
      a_cols    = a->getNumCols(),
      b_rows    = b->getNumRows(),
      b_cols    = b->getNumCols(),
      c_rows    = c->getNumRows(),
      tilesPerDim = sqrt(size), // # of tiles along each row/column
      maxDim    = std::max(a_rows, std::max(a_cols, std::max(b_rows, b_cols))),
      tileDim   = ceil(maxDim / tilesPerDim),
      numTiles  = size; // one dimension of each tile (they're square)

  double * aLocalTile = new double[tileDim*tileDim],
         * bLocalTile = new double[tileDim*tileDim];
  
  for (int i = 0; i < a_rows; ++i) {
    for (int j = 0; j < a_cols; ++j) {
        a->setIJ(i,j,i*a_cols+j);
        b->setIJ(i,j,i*a_cols+j);
    }
  }

  if (rank==0){
    a->printMatrix('\t');
    std::cout << std::endl;
    b->printMatrix('\t');
    std::cout << std::endl;
  }
  
  double* a_raw = a->getMatrix(),
        * b_raw = b->getMatrix(),
        * c_raw = c->getMatrix();


  dataManager.startTimer();

  size_t iterationsNeeded = tilesPerDim;

  int startIdx = getStartIndex(rank, maxDim, tileDim),
      tileRowIdx = rank / tilesPerDim,
      tileColIdx = rank % tilesPerDim;

  /*
  bool print = false;

  if (rank==0) print = true;
  
  if (print){
    b->printMatrix('\t');
  }

  rotate(rank, tileDim, tilesPerDim, a_raw, b_raw, a_rows, b_rows, maxDim);
  
  consolidateC(rank, maxDim, tileDim, numTiles, b_raw);
  if (print){
    std::cout << std::endl;
    b->printMatrix('\t');
  }
  */

  // pointers starting at our needed offsets
  double  * localA = &a_raw[startIdx],
          * localB = &b_raw[startIdx],
          * localC = &c_raw[startIdx];

  for (size_t iter=0; iter<iterationsNeeded; ++iter){

    //#pragma omp parallel for num_threads(this->numThreads)
    for (size_t i=0; i<tileDim; ++i){
      if (i+tileRowIdx*tileDim > a_rows) continue; // verify i does not exceed the number of rows in A
      
      for (size_t k=0; k<tileDim; ++k){
        if (k+tileColIdx*tileDim > a_cols) continue; // verify k does not exceed the number of cols in A (rows in B)
        double r = localA[i*a_rows+k];

        for (size_t j=0; j<tileDim; ++j){
          if (j+tileColIdx*tileDim > b_cols) continue; // verify i does not exceed the number of rows in A
          localC[i*c_rows+j] += r * localB[k*b_rows+j];
        }
      }
    }
    if (size>1) rotate(rank, tileDim, tilesPerDim, a_raw, b_raw, a_rows, b_rows, maxDim);
  }

  dataManager.stopTimer();
  
  this->consolidateC(rank, maxDim, tileDim, numTiles, c_raw);
  
  MPI_Finalize();

  return c;
}

void MatrixMultiplication::rotate(int rank, int tileDim, int tilesPerDim, double* a, double* b, int rowsA, int rowsB, int maxDim){
  MPI_Status status;
  MPI_Datatype a_tile_type;
  MPI_Datatype b_tile_type;
  MPI_Type_vector(tileDim, tileDim, rowsA, MPI_DOUBLE, &a_tile_type);
  MPI_Type_vector(tileDim, tileDim, rowsB, MPI_DOUBLE, &b_tile_type);
  MPI_Type_commit(&a_tile_type);
  MPI_Type_commit(&b_tile_type);

  int startIdx        = getStartIndex(rank, maxDim, tileDim),
      rowBlockIdx     = rank / tilesPerDim,
      colBlockIndex   = rank % tilesPerDim,
      firstToLastOffset = (tilesPerDim-1)*tilesPerDim;

  // B exchanges/rotates vertically
  if (rowBlockIdx > 0) // all but the top row send up  
    MPI_Send(&b[startIdx], 1, b_tile_type, rank - tilesPerDim, 0, MPI_COMM_WORLD);

  if (rowBlockIdx == 0) // top row sends to the bottom
    MPI_Send(&b[startIdx], 1, b_tile_type, rank + firstToLastOffset, 0, MPI_COMM_WORLD);

  if (rowBlockIdx < tilesPerDim-1) // all but the bottom row receive from down
    MPI_Recv(&b[startIdx], 1, b_tile_type, rank + tilesPerDim, 0, MPI_COMM_WORLD, &status);
  
  if (rowBlockIdx == tilesPerDim-1) // bottom row receives from top
    MPI_Recv(&b[startIdx], 1, b_tile_type, rank - firstToLastOffset, 0, MPI_COMM_WORLD, &status);
  

  // A exchanges/rotates horizontally
  if (colBlockIndex > 0) // all but left blocks send to the left
    MPI_Send(&a[startIdx], 1, a_tile_type, rank - 1, 0, MPI_COMM_WORLD);

  if (colBlockIndex == 0) // left blocks send to the rightmost blocks
    MPI_Send(&a[startIdx], 1, a_tile_type, rank+tilesPerDim-1, 0, MPI_COMM_WORLD);

  if (colBlockIndex < tilesPerDim-1) // all but right blocks receive from the right
    MPI_Recv(&a[startIdx], 1, a_tile_type, rank + 1, 0, MPI_COMM_WORLD, &status); 
  
  if (colBlockIndex == tilesPerDim-1) // rightmost blocks receives the left most blocks
    MPI_Recv(&a[startIdx], 1, a_tile_type, rank-tilesPerDim+1, 0, MPI_COMM_WORLD, &status);
  

  MPI_Type_free(&a_tile_type);
  MPI_Type_free(&b_tile_type);
}

int MatrixMultiplication::getStartIndex(int rank, int maxDim, int tileDim){
  int numTilesPerDim = ceil(maxDim / tileDim),
      rowIdx         = rank / numTilesPerDim,
      rowOffset      = rank % numTilesPerDim;
  return tileDim*(rowIdx*maxDim + rowOffset);
}

void MatrixMultiplication::consolidateC(int rank, int maxDim, int tileDim, int numTiles, double* c){
  int startIdx = getStartIndex(rank, maxDim, tileDim);
  MPI_Status status;
  MPI_Datatype tile_type;
  // TODO maxDim should be cols of c
  MPI_Type_vector(tileDim, tileDim, maxDim, MPI_DOUBLE, &tile_type);
  MPI_Type_commit(&tile_type);

  if (rank>0)
    MPI_Send(&c[startIdx], 1, tile_type, 0, 0, MPI_COMM_WORLD);
  
  if (rank==0){
    for (size_t sendingRank=1; sendingRank<numTiles; ++sendingRank){
      int destinationIdx = getStartIndex(sendingRank, maxDim, tileDim);
      //std::cout << sendingRank << " " << destinationIdx << std::endl;
      MPI_Recv(&c[destinationIdx], 1, tile_type, sendingRank, 0, MPI_COMM_WORLD, &status);
    }
  }

  MPI_Type_free(&tile_type);
}

// leverages row major. Very good for C++.
Matrix* MatrixMultiplication::multiply(){
  int a_rows = a->getNumRows(),
      a_cols = a->getNumCols(),
      b_rows = b->getNumRows(),
      b_cols = b->getNumCols(),
      c_rows = c->getNumRows();
  
  /* same raw access as algorithm 0 */
  double* a_raw = a->getMatrix();
  double* b_raw = b->getMatrix();
  double* c_raw = c->getMatrix();

  
  dataManager.startTimer();

  #pragma omp parallel for num_threads(this->numThreads)
  for (int i=0; i<a_rows; ++i){
    for (int k=0; k<a_cols; ++k){
      double r = a_raw[i*a_rows+k];
      for (int j=0; j<b_cols; ++j){
        c_raw[i*c_rows+j] += r * b_raw[k*b_rows+j];
      }
    }
  }

  dataManager.stopTimer();

  return c;
}

bool MatrixMultiplication::verify(Matrix* m){
  this->multiply(); // run multiplication and overwrite the old copy of C with a verified algorithm
  
  for (size_t i=0; i<c->getNumRows(); ++i)
    for (size_t j=0; j<c->getNumCols(); ++j)
       if (this->c->getIJ(i,j) != m->getIJ(i,j)) return false;
    
  return true;
}