APP_NAME=suffixarray

OBJS += radixsort.o
OBJS += myersort.o
OBJS += skewalgorithm.o
OBJS += main.o

CXX = g++ -m64 -std=c++11
CXXFLAGS = -O3 -Wall -fopenmp -Wno-unknown-pragmas -DATOMIC_RADIX
# CXXFLAGS = -O3 -Wall -fopenmp -Wno-unknown-pragmas
# CXXFLAGS = -O3 -Wall -fopenmp -Wno-unknown-pragmas -ftree-vectorizer-verbose=1

default: $(APP_NAME)

$(APP_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

clean:
	/bin/rm -rf *~ *.o $(APP_NAME) *.class
