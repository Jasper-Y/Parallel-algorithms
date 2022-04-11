# Parallel Suffix Array Construction and Applications
Members: Chengji Yu (chengji2), Sree Revoori (srevoori)



## PROPOSAL

### Summary

We are going to implement a parallel version of algorithm(s) for work-efficient parallel suffix array construction on the CPU. Time permitting, we will implement applications of suffix arrays such as the Burrows-Wheeler transform.


### Background

A suffix array is a data structure that contains the sorted suffixes of a string. In contrast to suffix trees, they are extremely space efficient since we can compress each suffix to an integer, i.e. let the index the suffix starts on denote the suffix. As an example, say we want to construct a suffix tree for the string s = banana$. We first index each suffix based on the index it starts at.

| Suffix        | i           | 
| ------------- |:-----------:|
| banana$       | 0           |
| anana$        | 1           |   
| nana$         | 2           | 
| ana$          | 3           | 
| na$           | 4           | 
| a$            | 5           | 
| $             | 6           | 

After sorting the indices based on the suffix starting at that index, we get 

| Suffix        | i           | 
| ------------- |:-----------:|
| $             | 6           |
| a$            | 5           | 
| ana$          | 3           | 
| anana$        | 1           |
| banana$       | 0           |
| na$           | 4           | 
| nana$         | 2           | 

Thus, our suffix array is [6,5,3,1,0,4,2].

Although not immediately clear, suffix arrays have numerous applications in string problems such as longest palindromic substring, needle in haystack string search, longest common substring, and longest repeating substring. 

An O(n^2logn) algorithm is obvious - just find each substring and sort the array. The O(nlogn) algorithm is more subtle and involves a variation of radix sort. 

Many efficient sequential algorithms that compute this problem are recursive in nature and recursive algorithms that branch along one path often tend to be parallelizable. This is discussed further in the next section.


### Challenge

The challenge lies in the fact that even the sequential implementation of suffix array construction is very complicated and difficult to do in O(n). The reason for this is that there are O(n) suffixes for a string leading to O(n^2) total space for all the suffixes. However, this does not mean the lower bound for this problem is O(n^2). Because the suffixes share information, we can cleverly create the suffix array in O(nlogn) and even O(n). The traditional skew algorithm is recursive and admits the recurrence T(n) = T(2n/3) + O(n) which solves to T(n) = O(n). 

By the recurrence, we know there are O(log n) levels of recursion. This is one of the bottlenecks for this problem. Here, it is difficult to attain polylogarithmic span since we need a recurrence that looks like T(n) = T(2n/3) + o(n). To turn O(n) into o(n), we first use a parallel sorting algorithm that runs in O(log^2 n) span. 

Though the O(n) algorithm is optimal in sequential implementation, the fraction of sequential execution might prevent it from outperforming than parallel O(nlogn) algorithm. We are aiming at parallelizing on algorithms with different sequential time complexity to find the optimal parallel suffix array implementation. The ultimate goal is not limited to parallelizing O(n) algorithms, but is to find the most efficient solution given a specific number of threads/processors. The key work is to find the potential parallelism behind the algorithm.

### Resources

We plan to start from scratch. Here are the papers we will be referencing:

https://cp-algorithms.com/string/suffix-array.html (description of O(nlogn) algorithm

http://www.cs.cmu.edu/~guyb/paralg/papers/KarkkainenSanders03.pdf (original paper on skew algorithm)

http://algo2.iti.kit.edu/sanders/papers/KulSan06a.pdf (parallel version of skew algorithm)

https://arxiv.org/pdf/1610.08305.pdf (a very recent algorithm for O(n) construction with constant space)

We will be programming on the CPU on the GHC machines. 

### Goals and Deliverables

75% goal: Implement at least one parallel algorithm.

100% goal: Implement the parallel algorithm(s) and analyze performance characteristics. Optimize it so there is significant speedup and outperforms all sequential algorithms. 

125% goal: Implement the parallel algorithm(s) and analyze performance characteristics. Implement various applications such as the Burrows-Wheeler transform, suffix array to suffix tree conversion, and LCP array construction. 

At the final presentations, we will be showing graphs of the speedup based on different parameters (length of string, number of processors, gpu/cpu). We will compare this speedup to the sequential O(n^2 log n), O(nlogn), and O(n) algorithms. 

### Platform Choice

We will be doing this in C++ on the CPU. 

### Schedule

Week of 3/21: Implement the O(n^2logn) brute force and the O(nlogn) sequential algorithms

Week of 3/28: Complete the sequential skew algorithm and the (optional) algorithm described here - https://arxiv.org/pdf/1610.08305.pdf

Week of 4/4: Begin work on the parallel algorithm. 

Week of 4/11: Complete the parallel algorithm.

Week of 4/18: Make optimizations to parallel algorithm and conduct analysis and comparison of all the algorithms

Week of 4/25: Assuming we have met all our goals thus far, start working on applications. Otherwise, continue work from previous weeks to meet 100% goal.



## MILESTONE

### Summary

Suffix array construction has a considerable flexibility so that the algorithms vary a lot. Therefore the first step we did was to search different algorithms and understood the logic. Considering the time/space efficiency, productivity, conciseness, and potential attribute for parallelism, we implemented the sequential version of O(n<sup>2</sup>logn) brute force method, O(nlogn) Myers algorithm, O(nlogn) algorithm using radix sort, and O(n) skew algorithm. We set up the benchmark for the sequential performance. The test case was set as a random string with length 100,000.

| Algorithm              | O(n<sup>2</sup>logn) brute force | O(nlogn) Myers algorithm | O(nlogn) radix sorting | O(n) skew algorithm |
| ---------------------- | -------------------------------- | ------------------------ | ---------------------- | ------------------- |
| Sequential runtime (s) | 0.439395                         | 0.032200                 | 0.002397               | 0.004665            |

Finally before the milestone, we implemented the parallel version for the Myers algorithm and the speedups are as below.

| Number of threads | 2    | 4    | 8    |
| ----------------- | ---- | ---- | ---- |
| Speedup           | 1.33 | 2.06 | 3.23 |

### Updated goals

The general goals remain the same. We have done the algorithm search and obtained some preliminary results shown above. 

**At lease achieve:** We plan to keep optimizing the existing code to improve the sub-linear speedup. Some implementation details might have a negative impact on the benchmark result so we will also work on the code optimization from the point of system domain knowledge. 

**Plan to achieve:** We will also look for the intrinsicness of those algorithms we chose and test more parallel programming techniques with detailed analysis.

**Hope to achieve:** Ideally with some satisfying results, we will move on to the applications.

### Plan for poster session

**Plan to achieve:** We plan to show a graph(s) with performance analysis at the poster session. 

**Hope to achieve:** We hope to show a demo to demonstrate the significant advantage of using our parallelized algorithm.

### Issues

* Note that according to the benchmark, the Myers algorithm performs worse than the radix sorting though they have the same time complexity. This is probably related to implementation details. We have already adopted RB-tree for acceleration. This introduces some more work we have to deal with. One hypothesis is the iterative Myers algorithm requires a large queue to store the tasks, while the recursive Myers algorithm might have a really deep recursion stack. Therefore, the theoretical arithmatic time complexity is the same as radix sorting but actual behavior related to storage is worse.
* The skew algorithm we use is complex and did not perform well than the radix sorting. The implementation of skew algorithm is tricky so we do not plan to parallel the skew algorithm but will mainly focus on the radix sorting algorithm. We hope to use this comparison to prove our hypothesis that the solution with the optimal time complexity isn't always the best choice in parallel applications.

