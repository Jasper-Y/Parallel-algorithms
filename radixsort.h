#ifndef _RADIX_SORT_H
#define _RADIX_SORT_H

#include <string>
#include <vector>
#include <atomic>

// Counting sort
void counting_sort(int n, int bucket_size, std::vector<std::atomic<int>> &cnt,
                   std::vector<int> &res, const std::vector<int> &label,
                   const std::vector<int> &second_order);

// Return sorted suffix array by radix sorting algorithm
void sa_radixsort(const std::string &str, int n, std::vector<int> &order);

#endif // _RADIX_SORT_H
