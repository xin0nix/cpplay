// solution.hpp
#ifndef MINIMUM_COST_OF_ROPES_SOLUTION_H
#define MINIMUM_COST_OF_ROPES_SOLUTION_H

#include <cstdint>

// https://practice.geeksforgeeks.org/problems/minimum-cost-of-ropes-1587115620/1
// There are given N ropes of different lengths, we need to connect these ropes
// into one rope. The cost to connect two ropes is equal to sum of their
// lengths. The task is to connect the ropes with minimum cost. Given N size
// array arr[] contains the lengths of the ropes.

// I launched each kind of solution on geeks4geeks (in different times
// intervals) and got odd results which kind of correlate with the google
// benchmark, but not exactly.
// Oddly enough, the fastest solution does not involve using heap at all.
// On the second place is a "custom" min heap implementation.
// And the slowest one is the std heap (maybe because of the abstractions?).
enum class SolutionKind {
  // ~ 4,9 on geeks4geeks
  SK_MinHeap,
  // ~ 5,7 on geeks4geeks
  SK_STDMinHeap,
  // ~ 1.8 on geeks4geeks, why?
  SK_MapPQ
};

class Solution {
  // Function to return the minimum cost of connecting the ropes.
public:
  long long minCost(long long arr[], long long n,
                    SolutionKind sk = SolutionKind::SK_MinHeap);
};

#endif
