SRCDIR=.
BUILD=../build

PROGRAM=MatrixMultiplication
EXE=${BUILD}/${PROGRAM}

OPTLEVEL=
CXX=mpic++
CXXFLAGS=-std=c++11 -g -Wall $(OPTLEVEL)
LDLIBS=-fopenmp -lm

SRC:=$(shell find $(SRCDIR)/ -name "*.cpp")
OBJS:=$(SRC:%.cpp=$(BUILD)/%.o)

all: $(OBJS) $(BUILD)/$(PROGRAM)

$(BUILD)/$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $(BUILD)/$(OBJS) -o $(BUILD)/$(PROGRAM) ${LDLIBS}

$(BUILD)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< ${LDLIBS}

# sample command of how to call the program on matrix A = (10000, 10000) and B = (10000, 10000)
# with 5 MPI processes and 16 OpenMP threads PER process
run: all
	mpirun -np 5 --mca btl vader,self --bind-to socket --map-by socket ${EXE} -M 10000 -N 10000 -P 10000 -t 16

clean:
	rm -rf $(BUILD)/core* $(OBJS) $(EXE)
