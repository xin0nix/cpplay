#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/maximum-subarray/

struct SolutionImpl {
  virtual bool solve(std::vector<int> &nums) = 0;
};

struct BasicSolution : SolutionImpl {
  virtual bool solve(std::vector<int> &nums) override final;
};

struct Solution {
  bool solve(std::vector<int> &nums);
};
