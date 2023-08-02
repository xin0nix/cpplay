#pragma once

#include <vector>

// Problem statement:
// https://leetcode.com/problems/top-k-frequent-elements/submissions/

struct SolutionImpl {
  virtual std::vector<int> topKFrequent(std::vector<int> &nums, int k) = 0;
};

// Runtime 11ms Beats 97.09 %
// Memory 13.49mb Beats 95.50 %
struct BasicSolution : SolutionImpl {
  virtual std::vector<int> topKFrequent(std::vector<int> &nums,
                                        int k) override final;
};

struct Solution {
  std::vector<int> topKFrequent(std::vector<int> &nums, int k);
};
