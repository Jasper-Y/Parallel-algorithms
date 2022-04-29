APP_NAME=suffixarray

OBJS += radixsort.o
OBJS += myersort.o
OBJS += skewalgorithm.o
OBJS += main.o

CXX = g++ -m64 -std=c++11
# this is the normal usage
CXXFLAGS = -O3 -Wall -fopenmp -Wno-unknown-pragmas

# comment out this to test atomic operation in radix sort
# CXXFLAGS = -O3 -Wall -fopenmp -Wno-unknown-pragmas -DATOMIC_RADIX

# comment out this to test local accumulation with omp in radix sort
# CXXFLAGS = -O3 -Wall -fopenmp -Wno-unknown-pragmas -DOMP_RADIX

# comment out this to test without vectorization 
# CXXFLAGS = -O3 -Wall -fopenmp -Wno-unknown-pragmas -fno-tree-vectorize

# comment out this to see the log for vectorization result
# CXXFLAGS = -O3 -Wall -fopenmp -Wno-unknown-pragmas -ftree-vectorizer-verbose=1

default: $(APP_NAME)

$(APP_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

clean:
	/bin/rm -rf *~ *.o $(APP_NAME) *.class
