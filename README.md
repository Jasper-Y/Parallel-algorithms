# Parallel Suffix Array Construction and Applications
Members: Chengji Yu (chengji2), Sree Revoori (srevoori)



### Summary

We are going to implement a parallel version of the skew algorithm for work-efficient parallel suffix array construction on the CPU. Time permitting, we will implement applications of suffix arrays such as the Burrows-Wheeler transform.


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

Though the O(n) algorithm is optimal in sequential implementation, the fraction of sequential execution might prevent it from outperforming than parallel O(nlogn) algorithm. We are aiming at parallelizing on algorithms with different sequential time complexity to find the optimal parallel suffix array implementation.

### Resources

We plan to start from scratch. Here are the papers we will be referencing:

https://cp-algorithms.com/string/suffix-array.html (description of O(nlogn) algorithm

http://www.cs.cmu.edu/~guyb/paralg/papers/KarkkainenSanders03.pdf (original paper on skew algorithm)

http://algo2.iti.kit.edu/sanders/papers/KulSan06a.pdf (parallel version of skew algorithm)

https://arxiv.org/pdf/1610.08305.pdf (a very recent algorithm for O(n) construction with constant space)

We will be programming on the CPU on the GHC machines. 

### Goals and Deliverables

75% goal: Implement the parallel skew algorithm.

100% goal: Implement the parallel skew algorithm and analyze performance characteristics. Optimize it so there is significant speedup and outperforms all sequential algorithms. 

125% goal: Implement the parallel skew algorithm and analyze performance characteristics. Implement various applications such as the Burrows-Wheeler transform, suffix array to suffix tree conversion, and LCP array construction. 

At the final presentations, we will be showing graphs of the speedup based on different parameters (length of string, number of processors, gpu/cpu). We will compare this speedup to the sequential O(n^2 log n), O(nlogn), and O(n) algorithms. 

### Platform Choice

We will be doing this in C++ on the CPU. 

### Schedule

Week of 3/21: Implement the O(n^2logn) brute force and the O(nlogn) sequential algorithms

Week of 3/28: Complete the sequential skew algorithm and the (optional) algorithm described here - https://arxiv.org/pdf/1610.08305.pdf

Week of 4/4: Begin work on the parallel skew algorithm. 

Week of 4/11: Complete the parallel skew algorithm.

Week of 4/18: Make optimizations to parallel skew algorithm and conduct analysis and comparison of all the algorithms

Week of 4/25: Assuming we have met all our goals thus far, start working on applications. Otherwise, continue work from previous weeks to meet 100% goal.

