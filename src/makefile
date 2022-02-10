PROGRAM=MatrixMultiplication
OPTLEVEL=
CXX=g++
CXXFLAGS=-std=c++11 -g -Wall $(OPTLEVEL)
OBJS=ArgParser.o DataManager.o Matrix.o $(PROGRAM).o

$(PROGRAM):	$(OBJS) makefile
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(PROGRAM)

$(PROGRAM).o: $(PROGRAM).cpp $(PROGRAM).hpp makefile

ArgParser.o:	ArgParser.cpp ArgParser.hpp makefile

DataManager.o:	DataManager.cpp DataManager.hpp makefile

Matrix.o:	Matrix.cpp Matrix.hpp makefile

clean:
	rm -rf $(OBJS) core $(PROGRAM) *.log
