APP_NAME=suffixarray

OBJS += scan.o
OBJS += radixsort.o
OBJS += divideconquer.o
OBJS += skewalgorithm.o
OBJS += main.o

CXX = g++ -m64 -std=c++11
NVCC = nvcc
LDFLAGS = -L/usr/local/depot/cuda-10.2/lib64/ -lcudart
NVCCFLAGS = -O3 -m64 --gpu-architecture compute_61 -ccbin /usr/bin/gcc

OPTIONS = 

# this is the default usage
CXXFLAGS = -O3 -Wall -fopenmp -Wno-unknown-pragmas

CXXFLAGS += $(foreach OPTION,$(OPTIONS),$(OPTION))

# compile command: make OPTIONS="xx xx xx"
# replace xx with your options:
# -DATOMIC_RADIX: test atomic operation in radix sort. This option will ignore the below two.
# -DUSE_CUDA: use CUDA in radix sort
# -DOMP_RADIX: test local accumulation with omp in radix sort
# -fno-tree-vectorize: compile without vectorization
# -ftree-vectorizer-verbose=1: see the log for vectorization result

default: $(APP_NAME)

$(APP_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

%.o: %.cu
	$(NVCC) $< $(NVCCFLAGS) -c -o $@

clean:
	/bin/rm -rf *~ *.o $(APP_NAME) *.class
