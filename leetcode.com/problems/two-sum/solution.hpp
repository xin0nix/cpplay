#pragma once

#include <vector>

struct SolutionImpl {
  virtual std::vector<int> twoSum(std::vector<int> &nums, int target) = 0;
};

// Runtime 16ms Beats 62.33%
// Memory 10.94mb Beats 23.81%
struct MapSolution : SolutionImpl {
  virtual std::vector<int> twoSum(std::vector<int> &nums,
                                  int target) override final;
};

struct Solution {
  std::vector<int> twoSum(std::vector<int> &nums, int target);
};
