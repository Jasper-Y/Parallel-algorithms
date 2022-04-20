#include "radixsort.h"
#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <stdlib.h>

void counting_sort(int n, int bucket_size, std::vector<int> &cnt,
                   std::vector<int> &res, const std::vector<int> &label,
                   const std::vector<int> &second_order) {
    std::fill(cnt.begin(), cnt.end(), 0);
    for (int i = 0; i < n; i++) {
        cnt[label[i]]++;
    }
    for (int i = 1; i < bucket_size; i++) {
        // In our case, bucket size is usually alphabet size so parallel
        // exclusive prefix sum may not be effective. But parallel prefix sum is
        // definitely a great attempt.
        cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
        res[--cnt[label[i]]] = second_order[i];
    }
}

void sa_radixsort(const std::string &str, int n, std::vector<int> &order) {
    int bucket_size = std::max(n, 255);
    std::vector<int> cnt(bucket_size);
    // Same substring have the same lebel value
    std::vector<int> label(n);
    std::vector<int> reorder_label(n);
    // Distince values. The value for order[i] means the
    // substring(order[i]:) is the ith suffix currently. During the
    // iteration, same substrings are ordered by occurrence.
    std::vector<int> second_order(n);
    for (int i = 0; i < n; i++) {
        label[i] = (int)str[i];
    }
    std::iota(second_order.begin(), second_order.end(), 0);
    counting_sort(n, bucket_size, cnt, order, label, second_order);

    int len = 1;
    int p = 1;
    for (; p < n; len <<= 1) {
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
        memcpy(&reorder_label[0], &label[0], n * sizeof(int));
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
        bucket_size = p;
    }
}