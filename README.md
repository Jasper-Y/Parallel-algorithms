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

With more powerful computing machine and network, the latency of the web services has more attention from the users and some real-time application can be fulfilled such as robotic applications. Some of the algorithms in the heterogeneous database platforms have the library running in parallel but some not. Some companies in the industry also don't have enough humen resources to optimize their own library in parallel. 

In this project, we'd like to focus on developing parallel algorithms and applying on some applications. We will implement algorithms with different data structures, including lists, trees, and graphs. With different data structure, we are aiming at different algorithms, i.e., parallelizing sorting with lists/arrays, traversing on trees, searching on trees and graphs. Some of them have been optimized a lot by researchers. In that case, we will implement the algorithms first and analyze the speedup comparing with the simple sequential implementation. For those algorithms that are not generally adopted by the companies, we will also analyze the reason why they are not welcome by the industry and what they can benefit from utilizing our parallel implementation.

According to the actual real-world problem, we will choose whether to implement on both GPU and multi-core CPU, or only implement on one of them. For example, if the algorithm would be run on all the operation systems, including embedded system, it's more meaningful to optimize a version using SIMD (if applicable) rather than parallel on GPU. We will have discussion for the decision.

If the project goes pretty well, we also plan to apply two of them on some applications, implement the code, and visualize by using OpenGL.

We hope to see the tradeoff between code quality and speedup so the startup companies and the communities would be able to develop their own library with high parallelism performance, without need to find existing library with a lot of restricts.

### Challenge

The challenge lies in the fact that even the sequential implementation of suffix array construction is very complicated and difficult to do in O(n). The reason for this is that there are O(n) suffixes for a string leading to O(n^2) total space for all the suffixes. However, this does not mean the lower bound for this problem is O(n^2). Because the suffixes share information, we can cleverly create the suffix array in O(nlogn) and even O(n).  

Some of the specific algorithms we plan to implement are the following - 

Randomized Minimum Spanning Tree (MST) in Expected O(n) - http://www.cs.cmu.edu/~guyb/paralg/papers/KargerKleinTarjan95.pdf

Randomized Incremental Convex Hull - http://www.cs.cmu.edu/~guyb/paralg/papers/BlellochGuShunSun20.pdf

Parallel Cartesian Tree Construction - http://www.cs.cmu.edu/~guyb/paralg/papers/ShunBlelloch14.pdf

Parallel Strongly Connected Components (SCC) - http://www.cs.cmu.edu/~guyb/paralg/papers/CoppersmithFHP.pdf

Suffix Array Construction in O(n) - http://www.cs.cmu.edu/~guyb/paralg/papers/KarkkainenSanders03.pdf

We aim to finish at least 3 algorithms. Many of these algorithms have not been implemented before so this is an interesting problem. This project will highlight the profound relationship between randomized algorithms and work efficient parallel algorithms and the place that randomness and divide and conquer occupies within the realm of designing parallel algorithms. 

