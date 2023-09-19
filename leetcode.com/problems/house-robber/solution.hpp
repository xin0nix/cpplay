#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/house-robber/

struct SolutionImpl {
  virtual int rob(std::vector<int> &nums) = 0;
};

struct BFSolution : SolutionImpl {
  virtual int rob(std::vector<int> &nums) override final;
};

struct BottomUpSolution : SolutionImpl {
  virtual int rob(std::vector<int> &nums) override final;
};

struct Solution {
  int rob(std::vector<int> &&nums);
};
