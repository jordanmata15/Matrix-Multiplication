PROGRAM=MatrixMultiplication
OPTLEVEL=
CXX=g++
CXXFLAGS=-std=c++11 -g -Wall $(OPTLEVEL)

$(PROGRAM):	$(PROGRAM).o ArgParser.o DataManager.o Matrix.o makefile
	$(CXX) $(CXXFLAGS) $(PROGRAM).o ArgParser.o Matrix.o DataManager.o -o $(PROGRAM)

$(PROGRAM).o: $(PROGRAM).cpp $(PROGRAM).hpp makefile

ArgParser.o:	ArgParser.cpp ArgParser.hpp makefile

DataManager.o:	DataManager.cpp DataManager.hpp makefile

Matrix.o:	Matrix.cpp Matrix.hpp makefile

clean:
	rm -rf *.o core $(PROGRAM) *.log
