#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/min-cost-climbing-stairs/
struct SolutionImpl {
  virtual int minCostClimbingStairs(std::vector<int> &cost) = 0;
};

// results from leetcode ar not stable, runtime ranges from 0 to 0 ms, which
// makes no sense
struct BottomUpSolution : SolutionImpl {
  virtual int minCostClimbingStairs(std::vector<int> &cost) override final;
};

struct Solution {
  int minCostClimbingStairs(std::vector<int> cost);
};
