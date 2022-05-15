OPTLEVEL=
PROGRAM=MatrixMultiplication
CXX=mpic++
CXXFLAGS=-std=c++11 -g -Wall $(OPTLEVEL)
LDLIBS=-fopenmp -lm

# keep our cpp/hpp files separate from .o and executable
SRCDIR=.
BUILD=../build

SRC:=$(shell find $(SRCDIR)/ -name "*.cpp")
OBJS:=$(SRC:%.cpp=$(BUILD)/%.o)

all: $(OBJS) $(BUILD)/$(PROGRAM)

$(BUILD)/$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $(BUILD)/$(OBJS) -o $(BUILD)/$(PROGRAM) ${LDLIBS}

$(BUILD)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< ${LDLIBS}

run: all
	mpirun -np 4 ${BUILD}/${PROGRAM} -M 1000 -N 100 -P 2
# mpirun -np 4 ${BUILD}/${PROGRAM} -M 100 -N 1000 -P 10

clean:
	rm -rf $(BUILD)/core* $(OBJS) $(BUILD)/$(PROGRAM)