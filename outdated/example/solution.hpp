#pragma once

#include <vector>

// Problem statement: TODO

struct SolutionImpl {
  virtual bool solve(std::vector<int> &nums) = 0;
};

struct BasicSolution : SolutionImpl {
  virtual bool solve(std::vector<int> &nums) override final;
};

struct Solution {
  bool solve(std::vector<int> &nums);
};
