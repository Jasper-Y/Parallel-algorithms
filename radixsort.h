#ifndef _RADIX_SORT_H
#define _RADIX_SORT_H

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstring>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

// Return sorted suffix array by radix sorting algorithm
void sa_radixsort(const std::string &str, int n, std::vector<int> &order,
                  int num_threads, int8_t alphabet_size);

#endif // _RADIX_SORT_H
