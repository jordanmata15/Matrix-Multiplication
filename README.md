# Serial-Matrix-Multiplication

## Usage
On execution, users are prompted to enter the dimensions of the 2 matrices. There are 4 prompts total (one for each dimension). If any of the 4 dimensions entered is invalid, the program will reset all input and prompt from the beginning.

On successful input of all 4 dimensions, the program will generate 2 matrices based on the size of the 4 input. Let the dimensions entered for A be *n*,*m* and the dimensions entered for B be *p*,*q*. Then the program will randomly fill an *n*X*m* and a *p*X*q* matrix with values ranging from 0 to ULIMIT (Default=10, set in MatrixMultiplication.hpp).

The program will perform the matrix multiplication with each algorithm NUM_ITERS (default=3, set in MatrixMultiplication.hpp) amount of times and average the time taken by algorithm.

### Invalid Input
- Non integer characters
- Integers less than 1
- Inner dimensions unequal. ie. Let A be an *n*X*m* matrix and B be a *p*X*q* matrix. Invalid if *m*!=*p*.

## Notes
While it is better OOP practice to encapsulate the Matrix object and have setters and getters, we make the int** matrix public so that we can manipulate the raw pointers. This avoids the overhead of having to call something like getXY(x,y) for every access (*n*<sup>3</sup> times). For large *n*, this can be significant overhead.
