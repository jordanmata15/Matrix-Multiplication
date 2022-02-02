PROGRAM=MatrixMultiplication
CXX=g++
CXXFLAGS=-std=c++11 -g -Wall

$(PROGRAM):	$(PROGRAM).o ArgParser.o MatrixGenerator.o
	$(CXX) $(CXXFLAGS) $(PROGRAM).o ArgParser.o MatrixGenerator.o -o $(PROGRAM)

$(PROGRAM).o: $(PROGRAM).cpp

ArgParser.o:	ArgParser.cpp ArgParser.hpp

MatrixGenerator.o:	MatrixGenerator.cpp MatrixGenerator.hpp

clean:
	rm -rf *.o core $(PROGRAM)
