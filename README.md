# Parallel Suffix Array Construction and Applications
Members: Chengji Yu (chengji2), Sree Revoori (srevoori)



### Summary

We are going to implement the skew algorithm for work-optimal parallel suffix array construction on the CPU. Time permitting, we will implement applications of suffix arrays such as the Burrows-Wheeler transform.


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


### Challenge

The challenge lies in the fact that even the sequential implementation of suffix array construction is very complicated and difficult to do in O(n). The reason for this is that there are O(n) suffixes for a string leading to O(n^2) total space for all the suffixes. However, this does not mean the lower bound for this problem is O(n^2). Because the suffixes share information, we can cleverly create the suffix array in O(nlogn) and even O(n).  

### Resources

We plan to start from scratch. Here are the papers we will be referencing:

https://cp-algorithms.com/string/suffix-array.html (description of O(nlogn) algorithm

http://www.cs.cmu.edu/~guyb/paralg/papers/KarkkainenSanders03.pdf (original paper on skew algorithm)

http://algo2.iti.kit.edu/sanders/papers/KulSan06a.pdf (parallel version of skew algorithm)

https://arxiv.org/pdf/1610.08305.pdf (a very recent algorithm for O(n) construction with constant space)

We will be programming on the CPU on the GHC machines. 

