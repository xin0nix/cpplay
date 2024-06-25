#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/jump-game/

struct SolutionImpl {
  virtual bool canJump(std::vector<int> &nums) = 0;
};

// Leetcode score: Exceeded time limit :-(
struct DPSolution : SolutionImpl {
  virtual bool canJump(std::vector<int> &nums) override final;
};

// Runtime 46ms Beats 86.76 %
// Memory 48.49MB Beats 42.51 %
struct GreedySolution : SolutionImpl {
  virtual bool canJump(std::vector<int> &nums) override final;
};

struct Solution {
  bool canJump(std::vector<int> &nums);
};
