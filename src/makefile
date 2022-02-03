PROGRAM=MatrixMultiplication
CXX=g++
CXXFLAGS=-std=c++11 -g -Wall

$(PROGRAM):	$(PROGRAM).o ArgParser.o Matrix.o
	$(CXX) $(CXXFLAGS) $(PROGRAM).o ArgParser.o Matrix.o -o $(PROGRAM)

$(PROGRAM).o: $(PROGRAM).cpp

ArgParser.o:	ArgParser.cpp ArgParser.hpp

Matrix.o:	Matrix.cpp Matrix.hpp

clean:
	rm -rf *.o core $(PROGRAM)
