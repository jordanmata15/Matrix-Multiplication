# Serial-Matrix-Multiplication

## Overview
A program for multiplying 2 matrices (not necessarily square). Users provide the dimensions, after which, the program randomly fills them with an integer between 0-ULIMIT (ULIMIT is defined in MatrixMultiplication.hpp). After which, the program will perform the multiplication on three algorithms using different indexing for memory accesses. This shows the extra overhead incurred on how data is accessed. Since C++ is a row major language, we should be accessing contiguous rows of data whenever we can as opposed to switching between columns.

## The algorithms
- Algorithm 0: Uses *ijk* indexing. This uses contiguous row and column accesses. Because it uses both, it's not the worst.
- Algorithm 1: Uses *jki* indexing. This uses contiguous column accesses only. Each read is in a different row, so it has more overhead and is very slow. 
- Algorithm 2: Uses *ikj* indexing. This uses contiguous row accesses only. It leverages C++ row major structure so it is the fastest of the 3.

[Full Analysis](./Serial_Matrix_Multiplication_Report.pdf)

## Scripting Benchmarks
The makefile takes an optional parameter OPTLEVEL that lets you choose the optimization level for compilation. Running 
```
make OPTLEVEL=O3
```
will make all files with optimization level 3 (max). Not passing in the flag defaults to not passing in an optimization flag to the compiler.

## Usage

Mandatory flags with arguments:</br>
  &emsp;-M reads in rowsA         (rowsA          > 0, integer)</br>
  &emsp;-N reads in colsA/colsB   (colsA, colsB   > 0, integer)</br>
  &emsp;-P reads in colsB         (colsB          > 0, integer)</br>
Optional flags with arguments:</br>
  &emsp;-n reads in NUM_THREADS   (NUM_THREADS    > 0, integer)</br>
  &emsp;-p reads in ALG_NUM       (ALG_NUM either 0, 1, or 2)</br>
Optional flags without arguments:</br>
  &emsp;-a enables displaying matrices A/B one time each.</br>
  &emsp;-c enables displaying matrix C one time for each algorithm.</br>

eg. The following runs matrix multiplication on matrix A (300x100) and matrix B (100, 200)
```
./IntegralApproximation -M 300 -N 100 -P 200
```

## Notes
While it is better OOP practice to encapsulate the Matrix object and have setters and getters, we make the int** matrix public so that we can manipulate the raw pointers. This avoids the overhead of having to call something like getXY(x,y) for every access (*n*<sup>3</sup> times). For large *n*, this can be significant overhead. Seeing as how this is a benchmarking tool and ONLY a benchmarking tool, it can be overlooked.

It can be sped up even more by representing the 2D array as a single dimension array and using clever math to index the proper row and column.
