OPTLEVEL=
PROGRAM=MatrixMultiplication
CXX=g++
CXXFLAGS=-std=c++11 -g -Wall $(OPTLEVEL)
LDLIBS=-fopenmp

# keep our cpp/hpp files separate from .o and executable
SRCDIR=.
BUILD=../build

# https://stackoverflow.com/questions/48267813/makefile-compile-objects-to-different-directory-and-build-from-there
SRC:=$(shell find $(SRCDIR)/ -name "*.cpp")
OBJS:=$(SRC:%.cpp=$(BUILD)/%.o)

all: $(OBJS) $(BUILD)/$(PROGRAM)

$(BUILD)/$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $(BUILD)/$(OBJS) -o $(BUILD)/$(PROGRAM) ${LDLIBS}

$(BUILD)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< ${LDLIBS}

run: all
	$(BUILD)/$(PROGRAM) -M 100 -N 1000 -P 10

clean:
	rm -rf $(BUILD)/core* $(OBJS) $(BUILD)/$(PROGRAM)