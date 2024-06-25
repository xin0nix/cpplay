#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/contains-duplicate/

// TODO: implement using sorting, try benchmarking it properly

struct SolutionImpl {
  virtual bool containsDuplicate(std::vector<int> &nums) = 0;
};

// Leetcode score:
// Runtime: 154ms Beats 50.32 %
// Memory: 69.57mb Beats 46.36 %
struct SetSolution : SolutionImpl {
  virtual bool containsDuplicate(std::vector<int> &nums) override final;
};

// Leetcode score:
// Runtime: 144ms Beats 69.63 %
// Memory: 57.20mb Beats 94.55 %
struct SortSolution : SolutionImpl {
  virtual bool containsDuplicate(std::vector<int> &nums) override final;
};

struct Solution {
  bool containsDuplicate(std::vector<int> &nums);
};
