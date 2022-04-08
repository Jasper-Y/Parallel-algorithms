#include "radixsort.h"
#include "myersort.h"
#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std::chrono;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<double> dsec;

void print_array(const std::vector<int> &arr) {
    for (auto x : arr) {
        printf("%d ", x);
    }
    printf("\n");
}

std::vector<int> sa_serial(const std::string &str, int n) {
    std::vector<std::pair<std::string, int>> pairs;
    for (int i = 0; i < n; i++) {
        pairs.push_back({str.substr(i, n - i), i});
    }
    std::sort(pairs.begin(), pairs.end());
    std::vector<int> res;
    for (auto it : pairs) {
        res.push_back(it.second);
    }
    return res;
}

int main(int argc, char *argv[]) {
    int num_run = 100;
    int str_copy = 10;
    std::string str = "mississipi";
    for (int i = 0; i < str_copy; i++) {
        str += str;
    }
        int n = str.length();
    double serial_time = 0;
    double radix_time = 0;
    double myers_time = 0;
    auto start_time = Clock::now();

    // Serial
    std::vector<int> serial_output;
    start_time = Clock::now();
    for (int i = 0; i < num_run; i++) {
        serial_output = sa_serial(str, n);
    }
    serial_time += duration_cast<dsec>(Clock::now() - start_time).count();
    // for (auto i : serial_output) {
    //     std::cout << i << ": " << str.substr(i, n - i) << std::endl;
    // }

    // Serial radix
    std::vector<int> radixsort_output;
    start_time = Clock::now();
    for (int i = 0; i < num_run; i++) {
        radixsort_output = sa_radixsort(str, n);
    }
    radix_time += duration_cast<dsec>(Clock::now() - start_time).count();
    if (memcmp(&serial_output[0], &radixsort_output[0], n * sizeof(int)) != 0) {
        std::cout << "Wrong output for radixsort suffix array!" << std::endl;
        for (auto i : radixsort_output) {
            std::cout << i << ": " << str.substr(i, n - i) << std::endl;
        }
    }

    // Serial Myers
    std::vector<int> myersort_output;
    start_time = Clock::now();
    for (int i = 0; i < num_run; i++) {
        myersort_output = sa_myersort(str, n);
    }
    myers_time += duration_cast<dsec>(Clock::now() - start_time).count();
    if (memcmp(&serial_output[0], &myersort_output[0], n * sizeof(int)) != 0) {
        std::cout << "Wrong output for myers algorithm!" << std::endl;
        for (auto i : myersort_output) {
            std::cout << i << ": " << str.substr(i, n - i) << std::endl;
        }
    }

    printf("O(n^2logn) serial:\n%f\n", serial_time / num_run);
    printf("O(nlogn) serial radix:\n%f\n", radix_time / num_run);
    printf("O(nlogn) myers algorithm:\n%f\n", myers_time / num_run);
    return 0;
}
