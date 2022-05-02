# Parallel Suffix Array Construction
Members: Chengji Yu (chengji2), Sree Revoori (srevoori)



### Summary

We tried different parallel strategies on different algorithms for suffix array construction mainly on the CPU but also tried on GPU. Observing the speedup or the drop-off we got from different strategies, we proposed our explanation and our analysis on the trade-off between simple parallelizable algorithms and elaborate serial algorithms with high-efficiency.


### Background

##### Problem introduction

A suffix array is a data structure that contains the sorted suffixes of a string. In contrast to suffix trees, they are extremely space efficient since we can compress each suffix to an integer, i.e. let the index the suffix starts on denote the suffix. As an example, say we want to construct a suffix tree for the string `s = banana$`. We first index each suffix and represent by `S[i]` based on the index it starts at.

| S[i]    |  i   |
| ------- | :--: |
| banana$ |  0   |
| anana$  |  1   |
| nana$   |  2   |
| ana$    |  3   |
| na$     |  4   |
| a$      |  5   |
| $       |  6   |

After sorting the indices based on the suffix starting at that index, we get the sorted suffix array, represented by array `SA`.`SA[i]` means the index that the i-th smallest suffix starts on the input string. 

|  i   | SA[i] | S[SA[i]] |
| :--: | :---: | :------: |
|  0   |   6   |    $     |
|  1   |   5   |    a$    |
|  2   |   3   |   ana$   |
|  3   |   1   |  anana$  |
|  4   |   0   | banana$  |
|  5   |   4   |   na$    |
|  6   |   2   |  nana$   |

Thus, our suffix array `SA` is [6,5,3,1,0,4,2].

In this program, we construct a suffix array `SA` given a string. The scope and time complexity of a specific algorithm and parallel strategy could vary a lot with different input alphabet size. For conciseness, we use constant-sized alphabet, i.e., positive 8-bit integer to represent each character in the input. As a result, the data structures for different algorithms and parallel strategies are the same - an `std::string` and an `std::vector` of integers.

Although not immediately clear, suffix arrays have numerous applications in string problems such as longest palindromic substring, needle in haystack string search, longest common substring, and longest repeating substring. 

##### Algorithms

We adopted four algorithms and implemented the sequential version of them.

* Brute force sorting

  An O(n<sup>2</sup>logn) algorithm is obvious - just find each substring and sort the array. The sorting for an array with N elements requires O(nlogn) comparison and each comparison for two substrings is in O(n). The brute force sorting time scales super-linearly with the input size N. Therefore, the time cost for this algorithm might be very large even it reaches a fairly good speedup with parallelism. We will only use this implementation as a reference for other algorithms.

  Though the algorithm is simple, the implementation can significantly reduce the performance. The code we implemented before the milestone used literally substrings for comparison so the memory cost would be nontrivial. By optimizing the data structure and overriding the comparison function, the time cost for constructing a random string was reduced from ~0.5 seconds to less than 0.1 seconds.

* Divide and conquer

  This algorithm uses the idea of divide and conquer to realize an O(nlog<sup>2</sup>n) to at worst O(n<sup>2</sup>log<sup>2</sup>n) time complexity (depends on different problem sets). Different from brute force sorting, we have a list of target suffixes to sort. Then we first sort the suffixes by a specific range of characters in the suffix. We use a red-black tree to store the sorting result and each leaf on the tree have a list to store suffixes. The suffixes fall into the same leaf on the RB tree should have exactly the same substring. So for each element in the target suffixes list, we look up the key (i.e., the substring in particular range). If found, we add this suffix into the corresponding leaf's list. Otherwise, we insert a new leaf with the substring as the key. Finally, we traverse the RB tree. If the length of the list is one, then the suffix in that list is unique, which means the order of this suffix is determined. Otherwise, we save the list of suffix in the leaf as the target suffixes list in the next iteration.

  After each iteration (RB tree construction and traversal), we move the index of the substring range to the right of the previous range and double the length of substring. For example, when we are operating the suffix `S[k] = str.substr(k, len=n - k)`, in the first iteration, the length of the substring is 1 and the substring we use as the key for RB tree is `str[k]`. If the list of suffixes where `S[k]` exists has other suffixes, then this list would be pushed to the queue for the second iteration. In the second iteration, the length of substring is 2 and we use `str[k+1], ..., str[k+2]` as the key. Then we use `str[k+3], ..., str[k+6]`, `str[k+7], ..., str[k+14]` as the keys if these keys are also the substring of some other suffixes.

  The computational expense is mainly about storage. We tried both iterative and recursive implementation. The large queue for iteration and the overhead for deep recursion would influence the performance. Though the two have similar performance after optimization, it still requires more memory operation than any in-place sorting.

  The good thing is the obvious potential parallelism between subtasks. If the length of the list on the leaf is one, then there won't be any further task. If not, then the new target list of suffixes is unique among all the leaves and the tasks should be fully independent. Therefore, simple multi-tasks parallelism should work. In general cases, the sequential part is only the first RB tree construction and the propotion of parallelism should be fairly high.

* Construct with radix sorting

  In this algorithm, the O(nlogn) time complexity benefits from the idea of radix sorting. Different from the decimal digits, here we use the order of the substring as the label for radix sorting. Therefore, after the last sorting, the labels for the suffixes should be all unique and should be a permutation of `[0, ..., n - 1]`. At first, the length of substring is 1 and we label each suffix by this 1-length substring. By using counting sort (bucket sort), we reorder the labels in a continuous integer array starting from 0. Then we double the length and the labels for the suffixes are composed by two labels from the last iteration. This is similar to the higher digit and lower digit in radix sort. We have O(logn) iterations by doubling the length after iterations. By virtue of radix sort and counting sort, each iteration requires several O(n) traversals on all the suffixes. In total, it requires a stable O(nlogn) time.

  The algorithm implementation is more subtle and the data has strong connection with each other. Many `for` loops require to be run in order. That's being said, the data has a strong dependency. This not only makes it difficult to parallel, but also introduces cache conflicts to lower the speed. The parallelizable part of the implementation still exists and it has the potential to benefit from SIMD execution, vectorization, GPU execution, etc. 

* Skew algorithm

  This O(n) algorithm is very complicated and difficult to reproduce even the sequential version. Basically it comes up with the idea to divide the targets by moduloing 3. It sorts one of them with recursion inside the sorting algorithm. Then it utilizes the result to sort the others and finally merges the three parts using a specific comparator.

  This algorithm must be implemented carefully. Because of the explicit division of work, it has the potential parallelism between tasks. However, considering the productivity and the scope of this project, we will use this algorithm as a performance reference but won't implement the parallel version.



