#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/maximum-subarray/

struct SolutionImpl {
  virtual int maxSubArray(std::vector<int> &nums) = 0;
};

// Time Limit Exceeded (too bad)
struct BruteForceSolution : SolutionImpl {
  virtual int maxSubArray(std::vector<int> &nums) override final;
};

// Runtime: 121ms Beats 67.84%
// Memory: 67.69mb Beats 79.93%
struct DivideConquerSolution : SolutionImpl {
  virtual int maxSubArray(std::vector<int> &nums) override final;
};

// Runtime: 78ms Beats 99.56%
// Memory: 67.77mb Beats 79.93%
struct SlidingWindowSolution : SolutionImpl {
  virtual int maxSubArray(std::vector<int> &nums) override final;
};

struct Solution {
  int maxSubArray(std::vector<int> &nums);
};
