#include "myersort.h"


std::vector<int> sa_myersort(const std::string &str, int n) {
    int bucket_size = std::max(n, 255);
    std::vector<int> cnt(bucket_size);
    // Same substring have the same lebel value
    std::vector<int> label(n);
    std::vector<int> reorder_label(n);
    // Distince values. The value for order[i] means the
    // substring(order[i]:) is the ith suffix currently. During the
    // iteration, same substrings are ordered by occurrence.
    std::vector<int> order(n);
    std::vector<int> second_order(n);
    for (int i = 0; i < n; i++) {
        label[i] = (int)str[i];
    }
    
    return order;
}