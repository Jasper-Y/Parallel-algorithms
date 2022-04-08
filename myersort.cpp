#include "myersort.h"
#include <map>

void sort_subset(const std::string &str, const std::vector<int> &target,
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
            sort_subset(str, iter->second, result, start_pos + num, len << 1);
        }
        num += iter->second.size();
    }
}

std::vector<int> sa_myersort(const std::string &str, int n) {
    // Distince values. The value for order[i] means the
    // substring(order[i]:) is the ith suffix currently.
    std::vector<int> order(n);

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
    for (auto iter = groups.begin(); iter != groups.end(); iter++) {
        if (iter->second.size() == 1) {
            order[num] = iter->second[0];
        } else {
            sort_subset(str, iter->second, order, num, 1);
        }
        num += iter->second.size();
    }

    return order;
}