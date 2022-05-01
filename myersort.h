#ifndef _MYERS_SORT_H
#define _MYERS_SORT_H

#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <omp.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

void sa_myersort(const std::string &str, int n, std::vector<int> &output,
                 int num_threads);

#endif // _MYERS_SORT_H
