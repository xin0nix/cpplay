#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/jump-game-ii/
struct SolutionImpl {
  virtual int jump(std::vector<int> &nums) = 0;
};

// Runtime 15ms Beats 67.98 %
// Memory 16.70MB Beats 44.83 %
struct GreedySolution : SolutionImpl {
  virtual int jump(std::vector<int> &nums) override final;
};

struct Solution {
  int jump(std::vector<int> &nums);
};
