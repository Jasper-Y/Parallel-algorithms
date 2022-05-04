#include "divideconquer.h"

using namespace std::chrono;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<double> dsec;

struct comp {
    bool operator()(const std::pair<const char *, int> &lhs,
                    const std::pair<const char *, int> &rhs) {
        return memcmp(lhs.first, rhs.first, lhs.second * sizeof(char)) < 0;
    }
};

void sort_subset_recurse(const std::string &str, const std::vector<int> &target,
                         std::vector<int> &result, int start_pos, int len) {
    std::map<std::string, std::vector<int>> groups;
    for (auto x : target) {
        std::string s = str.substr(x + len, len);
        auto iter = groups.find(s);
        if (iter != groups.end()) {
            iter->second.push_back(x);
        } else {
            groups.insert(std::pair<std::string, std::vector<int>>(
                s, std::vector<int>(1, x)));
        }
    }
    int num = 0;
    for (auto iter = groups.begin(); iter != groups.end(); iter++) {
        if (iter->second.size() == 1) {
            result[start_pos + num] = iter->second[0];
        } else {
            sort_subset_recurse(str, iter->second, result, start_pos + num,
                                len << 1);
        }
        num += iter->second.size();
    }
}

// Iteration outperforms recursion slightly
void sort_subset_iterate(const std::string &str, const std::vector<int> &target,
                         std::vector<int> &result, int start_pos, int len) {
    std::queue<std::pair<int, std::vector<int>>> task_queue;
    task_queue.push(std::pair<int, std::vector<int>>(start_pos, target));

    while (!task_queue.empty()) {
        int q_len = task_queue.size();
        while (q_len-- > 0) {
            std::pair<int, std::vector<int>> new_task = task_queue.front();
            task_queue.pop();

            std::map<std::pair<const char *, int>, std::vector<int>, comp>
                groups;
            for (auto x : new_task.second) {
                std::pair<const char *, int> p =
                    std::make_pair(&str[x + len], len);
                auto iter = groups.find(p);
                if (iter != groups.end()) {
                    iter->second.push_back(x);
                } else {
                    groups.insert(
                        std::pair<std::pair<const char *, int>,
                                  std::vector<int>>(p, std::vector<int>(1, x)));
                }
            }
            int num = 0;
            for (auto iter = groups.begin(); iter != groups.end(); iter++) {
                if (iter->second.size() == 1) {
                    result[new_task.first + num] = iter->second[0];
                } else {
                    task_queue.push(std::pair<int, std::vector<int>>(
                        new_task.first + num, iter->second));
                }
                num += iter->second.size();
            }
        }
        len <<= 1;
    }
}

void sa_divideconquer(const std::string &str, int n, std::vector<int> &output,
                      int num_threads) {
    // Distince values. The value for output[i] means the
    // substring(output[i]:) is the ith suffix currently.
    omp_set_num_threads(num_threads);

    // auto start_time = Clock::now();
    std::map<char, std::vector<int>> groups;
    for (int i = 0; i < n; i++) {
        char s = str[i];
        auto iter = groups.find(s);
        if (iter != groups.end()) {
            iter->second.push_back(i);
        } else {
            groups.insert(
                std::pair<char, std::vector<int>>(s, std::vector<int>(1, i)));
        }
    }
    int num = 0;
    std::vector<std::pair<int, std::vector<int>>> next_round;
    for (auto iter = groups.begin(); iter != groups.end(); iter++) {
        if (iter->second.size() == 1) {
            output[num] = iter->second[0];
        } else {
            next_round.push_back(
                std::pair<int, std::vector<int>>(num, iter->second));
        }
        num += iter->second.size();
    }

    // double serial_time = duration_cast<dsec>(Clock::now() - start_time).count();

    // printf("Serial part in the divide conquer algorithm: %lf\n", serial_time);

    int i = 0;
    int num_task = next_round.size();
#pragma omp parallel for private(i) schedule(dynamic)
    for (i = 0; i < num_task; i++) {
        sort_subset_iterate(str, next_round[i].second, output,
                            next_round[i].first, 1);
    }
}