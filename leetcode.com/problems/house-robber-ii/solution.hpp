#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/house-robber-ii/

struct SolutionImpl {
  virtual int rob(std::vector<int> &nums) = 0;
};

struct BottomUpSolution : SolutionImpl {
  virtual int rob(std::vector<int> &nums) override final;
};

struct Solution {
  int rob(std::vector<int> &&nums);
};
