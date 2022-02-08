#!/bin/bash

# Where to redirect build messages
BUILD_OUTPUT="/dev/null"
#BUILD_OUTPUT=/dev/stdout

SRC_DIR=../src

echo "*******************************************"
echo "     Running With Optimization level 0"
echo "*******************************************"
make clean -C $SRC_DIR > $BUILD_OUTPUT
make OPTLEVEL=-O0 -C $SRC_DIR > $BUILD_OUTPUT
cat run_config.txt | $SRC_DIR/MatrixMultiplication
echo

echo "*******************************************"
echo "     Running With Optimization level 3"
echo "*******************************************"
make clean -C $SRC_DIR > $BUILD_OUTPUT
make OPTLEVEL=-O3 -C $SRC_DIR > $BUILD_OUTPUT
cat run_config.txt | $SRC_DIR/MatrixMultiplication
echo
