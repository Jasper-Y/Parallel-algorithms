#include "myersort.h"
#include "radixsort.h"
#include "skewalgorithm.h"
#include <algorithm>
#include <chrono>
#include <cstring>
#include <getopt.h>
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

void check_result(const std::vector<int> &expect,
                  const std::vector<int> &result, int n,
                  const std::string &info) {
    if (memcmp(&expect[0], &result[0], n * sizeof(int)) != 0) {
        std::cout << "Wrong output! Detail: " << info << std::endl;
    }
}

bool cmp_function (std::pair<int, const char*> a, std::pair<int, const char*> b) {
    return strcmp(a.second + a.first, b.second + b.first) < 0;
}

void sa_serial(const std::string &str, int n, std::vector<int> &output) {
    std::vector<std::pair<int, const char*>> pairs(n);
    // std::vector<std::pair<std::string, int>> pairs;
    for (int i = 0; i < n; i++) {
        pairs[i] = std::make_pair(i, &str[0]);
    }
    std::sort(pairs.begin(), pairs.end(), cmp_function);
    for (int i = 0; i < n; i++) {
        output[i] = pairs[i].first;
    }
}

void usage(const char *progname) {
    printf("Usage: %s [options]\n", progname);
    printf("Program Options:\n");
    printf("  -t  --threads <N>       Use N threads\n");
    printf("  -s  input string        Specify input string. No longer than "
           "256. \n");
    printf("  -r  --repeat <INT>      Specify double the string how many "
           "times. Actually length would be len(input) * 2 ^ r.\n");
    printf("  -?  --help              This message\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int num_run = 1;
    int str_copy = 0;
    int num_threads = 1;
    // std::string str = "mississipi";
    // std::string str = "abcdefghijklmnopqrstuvwxyz";
    std::string str = "";
    for (int i = 0; i < 50000; i++) {
        // str += (int)(rand() / (RAND_MAX + 1.0) * 26) + 'a';
        str += 'a';
    }
    str += 'b';

    // parse commandline options
    int opt;
    static struct option long_options[] = {{"threads", 1, 0, 't'},
                                           {"input_string", 1, 0, 's'},
                                           {"repeat_string", 1, 0, 'r'}};
    while ((opt = getopt_long(argc, argv, "t:s:r:?", long_options, NULL)) !=
           EOF) {
        switch (opt) {
        case 't': {
            num_threads = atoi(optarg);
            num_threads = num_threads > 0 ? num_threads : 1;
            break;
        }
        case 's': {
            char input_string[256];
            strcpy(input_string, optarg);
            str = std::string(input_string);
            break;
        }
        case 'r': {
            str_copy = atoi(optarg);
            break;
        }
        case '?':
        default:
            usage(argv[0]);
            return 1;
        }
    }

    for (int i = 0; i < str_copy; i++) {
        str += str;
    }

    int n = str.length();
    double serial_time = 0;
    double radix_time = 0;
    double myers_time = 0;
    double skew_time = 0;
    std::vector<int> serial_output(n);
    std::vector<int> radix_output(n);
    std::vector<int> myers_output(n);
    std::vector<int> skew_output(n + 3);
    auto start_time = Clock::now();

    // Serial
    start_time = Clock::now();
    for (int i = 0; i < num_run; i++) {
        sa_serial(str, n, serial_output);
    }
    serial_time += duration_cast<dsec>(Clock::now() - start_time).count();
    printf("O(n^2logn) serial:\n%f\n", serial_time / num_run);
    // for (auto i : serial_output) {
    //     std::cout << i << ": " << str.substr(i, n - i) << std::endl;
    // }

    // Serial radix
    #if defined(ATOMIC_RADIX)
        printf("Use atomic addition inside radix:\n");
    #endif
    start_time = Clock::now();
    for (int i = 0; i < num_run; i++) {
        sa_radixsort(str, n, radix_output, num_threads);
    }
    radix_time += duration_cast<dsec>(Clock::now() - start_time).count();
    printf("O(nlogn) serial radix:\n%f\n", radix_time / num_run);
    check_result(serial_output, radix_output, n, "radixsort");

    // Serial Myers
    start_time = Clock::now();
    for (int i = 0; i < num_run; i++) {
        sa_myersort(str, n, myers_output, num_threads);
    }
    myers_time += duration_cast<dsec>(Clock::now() - start_time).count();
    check_result(serial_output, myers_output, n, "myers algorithm");
    printf("O(nlogn) myers algorithm:\n%f\n", myers_time / num_run);

    // Serial skew
    start_time = Clock::now();
    for (int i = 0; i < num_run; i++) {
        sa_skew(str, n, skew_output);
    }
    skew_time += duration_cast<dsec>(Clock::now() - start_time).count();
    check_result(serial_output, skew_output, n, "skew algorithm");
    printf("O(n) skew algorithm:\n%f\n", skew_time / num_run);

    return 0;
}
