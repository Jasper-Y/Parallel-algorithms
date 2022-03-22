# Parallel algorithms and applications
Members: Chengji Yu (chengji2), Sree Revoori (srevoori)



### Summary

We are going to implement several parallel algorithms including sorting, traversing, and searching, and analyze two applications of them on web service and robot applications. According to specific real-world problem, we will implement on either both GPU and multi-core modern CPU, or one of them.



### Background

With more powerful computing machine and network, the latency of the web services has more attention from the users and some real-time application can be fulfilled such as robotic applications. Some of the algorithms in the heterogeneous database platforms have the library running in parallel but some not. Some companies in the industry also don't have enough humen resources to optimize their own library in parallel. 

In this project, we'd like to focus on developing parallel algorithms and applying on some applications. We will implement algorithms with different data structures, including lists, trees, and graphs. With different data structure, we are aiming at different algorithms, i.e., parallelizing sorting with lists/arrays, traversing on trees, searching on trees and graphs. Some of them have been optimized a lot by researchers. In that case, we will implement the algorithms first and analyze the speedup comparing with the simple sequential implementation. For those algorithms that are not generally adopted by the companies, we will also analyze the reason why they are not welcome by the industry and what they can benefit from utilizing our parallel implementation.

According to the actual real-world problem, we will choose whether to implement on both GPU and multi-core CPU, or only implement on one of them. For example, if the algorithm would be run on all the operation systems, including embedded system, it's more meaningful to optimize a version using SIMD (if applicable) rather than parallel on GPU. We will have discussion for the decision.

If the project goes pretty well, we also plan to apply two of them on some applications, implement the code, and visualize by using OpenGL.

We hope to see the tradeoff between code quality and speedup so the startup companies and the communities would be able to develop their own library with high parallelism performance, without need to find existing library with a lot of restricts.

