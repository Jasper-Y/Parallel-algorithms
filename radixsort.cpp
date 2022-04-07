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

void counting_sort(int n, int bucket_size, std::vector<int> &cnt,
                   std::vector<int> &res, const std::vector<int> &label,
                   const std::vector<int> &second_order) {
    std::fill(cnt.begin(), cnt.end(), 0);
    for (int i = 0; i < n; i++) {
        cnt[label[i]]++;
    }
    for (int i = 1; i < bucket_size; i++) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
        res[--cnt[label[i]]] = second_order[i];
    }
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

std::vector<int> sa_radixsort(const std::string &str, int n) {
    int bucket_size = std::max(n, 255);
    std::vector<int> cnt(bucket_size);
    // Same substring have the same lebel value
    std::vector<int> label(bucket_size);
    std::vector<int> reorder_label(bucket_size);
    // Distince values. The value for order[i] means the
    // substring(order[i]:) is the ith suffix currently. During the
    // iteration, same substrings are ordered by occurrence.
    std::vector<int> order(n);
    std::vector<int> second_order(n);
    for (int i = 0; i < n; i++) {
        label[i] = (int)str[i];
    }
    std::iota(second_order.begin(), second_order.end(), 0);
    counting_sort(n, bucket_size, cnt, order, label, second_order);

    int len = 1;
    int p = 1;
    for (; p < n; len <<= 1) {
        bucket_size = p;
        p = 0;

        for (int i = n - len; i < n; i++) {
            second_order[p++] = i;
        }
        for (int i = 0; i < n; i++) {
            if (order[i] >= len) {
                second_order[p++] = order[i] - len;
            }
        }
        for (int i = 0; i < n; i++) {
            reorder_label[i] = label[second_order[i]];
        }
        counting_sort(n, bucket_size, cnt, order, reorder_label, second_order);

        p = 1;
        // reorder_label is no longer useful, copy label into it to save memory
        memcpy(&reorder_label[0], &label[0], bucket_size * sizeof(int));
        label[order[0]] = 0;
        for (int i = 1; i < n; i++) {
            if (reorder_label[order[i - 1]] == reorder_label[order[i]] &&
                reorder_label[order[i - 1] + len] ==
                    reorder_label[order[i] + len]) {
                label[order[i]] = p - 1;
            } else {
                label[order[i]] = p++;
            }
        }
    }
    return order;
}

int main(int argc, char *argv[]) {
    std::string str = "mississipi";
    int n = str.length();

    std::vector<int> serial_output = sa_serial(str, n);
    for (auto i : serial_output) {
        std::cout << i << ": " << str.substr(i, n - i) << std::endl;
    }

    std::vector<int> radixsort_output = sa_radixsort(str, n);
    if (memcmp(&serial_output[0], &radixsort_output[0], n * sizeof(int)) != 0) {
        std::cout << "Wrong output for radixsort suffix array!" << std::endl;
        for (auto i : radixsort_output) {
            std::cout << i << ": " << str.substr(i, n - i) << std::endl;
        }
    }
}