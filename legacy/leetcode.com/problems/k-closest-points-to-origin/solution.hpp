// solution.hpp
#pragma once

#include <vector>

// See
// The problem:
// - https://leetcode.com/problems/k-closest-points-to-origin/
// Solutions:
// This one is a poor solution based on sorted multimap
// - https://www.geeksforgeeks.org/find-k-closest-points-to-the-origin/
// - https://www.youtube.com/watch?v=rI2EBUEMfTk
//

// TODO: implement max-heap version with the limited heap size
// TODO: compare different solutions using the benchmark

using namespace std;

struct KClosestSolutionInterface {
  virtual vector<vector<int>> kClosest(vector<vector<int>> &points, int k) = 0;
};

// Leetcode score:
// Final leet scope: Accepted
// Runtime: 290ms Beats 47.72 %
// Memory: 57.02mb Beats 95.44 %
struct MinHeapSolution : KClosestSolutionInterface {
  virtual vector<vector<int>> kClosest(vector<vector<int>> &points,
                                       int k) override final;
};

// Leetcode score:
// Runtime: 441ms Beats 17.96 %
// Memory: 64.39mb Beats 67.17 %
struct MaxHeapSolution : KClosestSolutionInterface {
  virtual vector<vector<int>> kClosest(vector<vector<int>> &points,
                                       int k) override final;
};

// Leetcode score:
// Runtime: 197ms, Beats 96.29 %
// Memory Details: 57.15mb, Beats 94.32 %
struct NthElementSolution : KClosestSolutionInterface {
  virtual vector<vector<int>> kClosest(vector<vector<int>> &points,
                                       int k) override final;
};

class Solution {
public:
  vector<vector<int>> kClosest(vector<vector<int>> &points, int k);
};
