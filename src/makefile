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


$(BUILD)/$(PROGRAM):	$(OBJS)
	$(CXX) $(CXXFLAGS) $(BUILD)/$(OBJS) -o $(BUILD)/$(PROGRAM) ${LDLIBS}

$(BUILD)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< ${LDLIBS}

clean:
	rm -rf $(BUILD)/core* $(OBJS) $(BUILD)/$(PROGRAM)