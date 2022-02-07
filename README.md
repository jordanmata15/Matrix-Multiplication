# Serial-Matrix-Multiplication

## Overview
A program for multiplying 2 matrices (not necessarily square). Users provide the dimensions, after which, the program randomly fills them with an integer between 0-ULIMIT (ULIMIT is defined in MatrixMultiplication.hpp). After which, the program will perform the multiplication on three algorithms using different indexing for memory accesses. This shows the extra overhead incurred on how data is accessed. Since C++ is a row major language, we should be accessing contiguous rows of data whenever we can as opposed to switching between rows.

On execution, it prompts the user for dimensions of matrices A and B. Then prompts the user: 
- Should A/B be printed out (once)?
- Should C should be printed out (once)? 
- Should the averages of each algorithm should be printed out? 
- How many runs of each iteration should occur for the averages?

## The algorithms
- Algorithm 0: Uses *ijk* indexing. This uses contiguous row and column accesses. Because it uses both, it's not the worst.
- Algorithm 1: Uses *jki* indexing. This uses contiguous column accesses only. Each read is in a different row, so it has more overhead and is very slow. 
- Algorithm 2: Uses *ikj* indexing. This uses contiguous row accesses only. It leverages C++ row major structure so it is the fastest of the 3.

## Notes
While it is better OOP practice to encapsulate the Matrix object and have setters and getters, we make the int** matrix public so that we can manipulate the raw pointers. This avoids the overhead of having to call something like getXY(x,y) for every access (*n*<sup>3</sup> times). For large *n*, this can be significant overhead.
