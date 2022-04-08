#ifndef _RADIX_SORT_H
#define _RADIX_SORT_H

#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

// Counting sort
void counting_sort(int n, int bucket_size, std::vector<int> &cnt,
                   std::vector<int> &res, const std::vector<int> &label,
                   const std::vector<int> &second_order);

// Return sorted suffix array by radix sorting algorithm
std::vector<int> sa_radixsort(const std::string &str, int n);

#endif // _RADIX_SORT_H
