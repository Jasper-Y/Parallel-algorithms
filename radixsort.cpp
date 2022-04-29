#include "radixsort.h"
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

typedef struct {
    int n;
    int start;
    int size;
    const int *label;
    std::atomic<int> *cnt;
} WorkerArgs;

void *counting_partial(void *threadArgs) {
    WorkerArgs *args = static_cast<WorkerArgs *>(threadArgs);
    int end = std::min(args->n, args->start + args->size);
    for (int i = args->start; i < end; i++) {
        args->cnt[args->label[i]].fetch_add(1, std::memory_order_seq_cst);
    }
    return NULL;
}

void counting_sort_atomic(int n, int bucket_size,
                          std::vector<std::atomic<int>> &cnt,
                          std::vector<int> &res, const std::vector<int> &label,
                          const std::vector<int> &second_order, int n_threads,
                          pthread_t workers[], WorkerArgs args[]) {
    for (int i = 0; i < bucket_size; i++) {
        cnt[i].store(0, std::memory_order_relaxed);
    }
    for (int i = 1; i < n_threads; i++) {
        pthread_create(&workers[i], NULL, counting_partial, &args[i]);
    }
    counting_partial(&args[0]);
    for (int i = 1; i < n_threads; i++) {
        pthread_join(workers[i], NULL);
    }

    for (int i = 1; i < bucket_size; i++) {
        // In our case, bucket size is usually alphabet size so parallel
        // exclusive prefix sum may not be effective. But parallel prefix sum is
        // definitely a great attempt.
        cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
        --cnt[label[i]];
        res[cnt[label[i]].load(std::memory_order_relaxed)] = second_order[i];
    }
}

void counting_sort(int n, int bucket_size, std::vector<int> &cnt,
                   std::vector<int> &res, const std::vector<int> &label,
                   const std::vector<int> &second_order) {
    std::fill(cnt.begin(), cnt.end(), 0);
    for (int i = 0; i < n; i++) {
        cnt[label[i]]++;
    }
    for (int i = 1; i < bucket_size; i++) {
        // In our case, bucket size is usually alphabet size so parallel
        // exclusive prefix sum may not be effective. But parallel prefix
        // sum is definitely a great attempt.
        cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
        res[--cnt[label[i]]] = second_order[i];
    }
}

void counting_sort_omp(int n, int bucket_size, std::vector<int> &cnt,
                       std::vector<int> &res, const std::vector<int> &label,
                       const std::vector<int> &second_order, int n_threads) {
    std::fill(cnt.begin(), cnt.end(), 0);
    int thread_size = (n + n_threads - 1) / n_threads;
    int i = 0;
#pragma omp parallel for private(i)
    for (i = 0; i < n_threads * thread_size; i += thread_size) {
        std::vector<int> local_cnt(bucket_size, 0);
        int start = i;
        int end = std::min(n, i + thread_size);
        for (int j = start; j < end; j++) {
            local_cnt[label[j]]++;
        }
        for (int k = 0; k < bucket_size; k++) {
#pragma omp atomic
            cnt[k] += local_cnt[k];
        }
    }

    /**
     * A second way for local accumulation
     *
    std::vector<std::vector<int>> local_cnt(n_threads,
                                            std::vector<int>(bucket_size, 0));
    #pragma omp parallel for
    for (int i = 0; i < n_threads; i++) {
        int start = i * thread_size;
        int end = std::min(n, start + thread_size);
        for (int j = start; j < end; j++) {
            local_cnt[i][label[j]]++;
        }
    }
    #pragma omp parallel for
    for (int i = 0; i < bucket_size; i++) {
        for (int j = 0; j < n_threads; j++) {
            cnt[i] += local_cnt[j][i];
        }
    }
    */

    for (int i = 1; i < bucket_size; i++) {
        // In our case, bucket size is usually alphabet size so parallel
        // exclusive prefix sum may not be effective. But parallel prefix
        // sum is definitely a great attempt.
        cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
        res[--cnt[label[i]]] = second_order[i];
    }
}

void sa_radixsort(const std::string &str, int n, std::vector<int> &order,
                  int num_threads) {
    int bucket_size = std::max(n, 255);
    // Same substring have the same label value
    std::vector<int> label(n);
    std::vector<int> reorder_label(n);
    // Distince values. The value for order[i] means the
    // substring(order[i]:) is the ith suffix currently. During the
    // iteration, same substrings are ordered by occurrence.
    std::vector<int> second_order(n);
    for (int i = 0; i < n; i++) { // Vectorized
        label[i] = (int)str[i];
    }
    std::iota(second_order.begin(), second_order.end(), 0); // Vectorized
#if defined(ATOMIC_RADIX)
    std::vector<std::atomic<int>> cnt(bucket_size);
    int thread_size = (n + num_threads - 1) / num_threads;
    pthread_t workers[num_threads];
    WorkerArgs args[num_threads];
    for (int i = 0; i < num_threads; i++) {
        args[i].n = n;
        args[i].start = thread_size * i;
        args[i].size = thread_size;
        args[i].cnt = &cnt[0];
        args[i].label = &label[0];
    }
#elif defined(OMP_RADIX)
    // std::vector<std::vector<int>> local_cnt(num_threads,
    // std::vector<int>(bucket_size));
    std::vector<int> cnt(bucket_size);
    omp_set_num_threads(num_threads);
#else
    std::vector<int> cnt(bucket_size);
#endif

#if defined(ATOMIC_RADIX)
    counting_sort_atomic(n, bucket_size, cnt, order, label, second_order,
                         num_threads, workers, args);
#elif defined(OMP_RADIX)
    counting_sort_omp(n, bucket_size, cnt, order, label, second_order,
                      num_threads);
#else
    counting_sort(n, bucket_size, cnt, order, label, second_order);
#endif

    int len = 1;
    int p = 1;
    for (; p < n; len <<= 1) {
        p = 0;

        for (int i = n - len; i < n; i++) { // Vectorized
            second_order[p++] = i;
        }
        for (int i = 0; i < n; i++) {
            if (order[i] >= len) {
                second_order[p++] = order[i] - len;
            }
        }
        // sometimes it's no benefit because of additional complications with
        // trying to combine the different data items and then sorting out what
        // goes where when the calculation is finished
        for (int i = 0; i < n; i++) {
            reorder_label[i] = label[second_order[i]];
        }
#if defined(ATOMIC_RADIX)
        counting_sort_atomic(n, bucket_size, cnt, order, reorder_label,
                             second_order, num_threads, workers, args);
#elif defined(OMP_RADIX)
        counting_sort_omp(n, bucket_size, cnt, order, reorder_label,
                          second_order, num_threads);
#else
        counting_sort(n, bucket_size, cnt, order, reorder_label, second_order);
#endif

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