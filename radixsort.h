#ifndef _RADIX_SORT_H
#define _RADIX_SORT_H

#include <string>
#include <vector>
#include <atomic>

// Return sorted suffix array by radix sorting algorithm
void sa_radixsort(const std::string &str, int n, std::vector<int> &order, int num_threads);

#endif // _RADIX_SORT_H
