make clean
make OPTLEVEL=-O0
cat run_config.txt | ./MatrixMultiplication
echo

make clean
make OPTLEVEL=-O3
cat run_config.txt | ./MatrixMultiplication
echo
