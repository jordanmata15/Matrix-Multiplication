PROGRAM=main
OPTLEVEL=
CXX=g++
CXXFLAGS=-std=c++11 -g -Wall $(OPTLEVEL)
OBJS=ArgParser.o DataManager.o Matrix.o MatrixMultiplication.o $(PROGRAM).o

$(PROGRAM):	$(OBJS) makefile
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(PROGRAM)

$(PROGRAM).o: $(PROGRAM).cpp $(PROGRAM).hpp

MatrixMultiplication.o:	MatrixMultiplication.cpp MatrixMultiplication.hpp

ArgParser.o:	ArgParser.cpp ArgParser.hpp

DataManager.o:	DataManager.cpp DataManager.hpp

Matrix.o:	Matrix.cpp Matrix.hpp

clean:
	rm -rf $(OBJS) core* $(PROGRAM) *.log
