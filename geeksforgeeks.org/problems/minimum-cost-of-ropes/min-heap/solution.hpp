// solution.hpp
#ifndef MINIMUM_COST_OF_ROPES_SOLUTION_H
#define MINIMUM_COST_OF_ROPES_SOLUTION_H

#include <cstdint>

// https://practice.geeksforgeeks.org/problems/minimum-cost-of-ropes-1587115620/1
// There are given N ropes of different lengths, we need to connect these ropes
// into one rope. The cost to connect two ropes is equal to sum of their
// lengths. The task is to connect the ropes with minimum cost. Given N size
// array arr[] contains the lengths of the ropes.

enum class SolutionKind { SK_MinHeap = 0, SK_MapPQ };

class Solution {
  // Function to return the minimum cost of connecting the ropes.
public:
  long long minCost(long long arr[], long long n,
                    SolutionKind sk = SolutionKind::SK_MinHeap);
};

#endif
