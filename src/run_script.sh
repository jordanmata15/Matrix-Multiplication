#!/bin/bash

# Where to redirect build messages
BUILD_OUTPUT="/dev/null"
#BUILD_OUTPUT=/dev/stdout


echo "*******************************************"
echo "     Running With Optimization level 0"
echo "*******************************************"
make clean > $BUILD_OUTPUT
make OPTLEVEL=-O0 > $BUILD_OUTPUT
cat run_config.txt | ./MatrixMultiplication
echo

echo "*******************************************"
echo "     Running With Optimization level 3"
echo "*******************************************"
make clean > $BUILD_OUTPUT
make OPTLEVEL=-O3 > $BUILD_OUTPUT
cat run_config.txt | ./MatrixMultiplication
echo
