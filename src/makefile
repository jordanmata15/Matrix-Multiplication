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

benchmark: all
	mpirun -np 1 ${BUILD}/${PROGRAM} -M 10000 -N 1000 -P 10000
	mpirun -np 2 ${BUILD}/${PROGRAM} -M 10000 -N 1000 -P 10000
	mpirun -np 4 ${BUILD}/${PROGRAM} -M 10000 -N 1000 -P 10000
	mpirun -np 8 ${BUILD}/${PROGRAM} -M 10000 -N 1000 -P 10000
	mpirun -np 16 ${BUILD}/${PROGRAM} -M 10000 -N 1000 -P 10000
	mpirun -np 32 ${BUILD}/${PROGRAM} -M 10000 -N 1000 -P 10000
	mpirun -np 64 ${BUILD}/${PROGRAM} -M 10000 -N 1000 -P 10000
	mpirun -np 128 ${BUILD}/${PROGRAM} -M 10000 -N 1000 -P 10000

clean:
	rm -rf $(BUILD)/core* $(OBJS) $(BUILD)/$(PROGRAM)