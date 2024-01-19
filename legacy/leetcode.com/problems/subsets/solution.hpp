#pragma once

#include <vector>

// Problem statement: TODO

struct SolutionImpl {
  virtual std::vector<std::vector<int>> subsets(std::vector<int> &nums) = 0;
};

// Runtime 3ms Beats 59.96%
// Memory 12.77mb Beats 40.08 %
struct BasicSolution : SolutionImpl {
  virtual std::vector<std::vector<int>>
  subsets(std::vector<int> &nums) override final;
};

struct Solution {
  std::vector<std::vector<int>> subsets(std::vector<int> &nums);
};
