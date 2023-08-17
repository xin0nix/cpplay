#pragma once

#include <vector>

// Problem statement:
// https://leetcode.com/problems/product-of-array-except-self/

struct SolutionImpl {
  virtual std::vector<int> productExceptSelf(std::vector<int> &nums) = 0;
};

struct BasicSolution : SolutionImpl {
  virtual std::vector<int>
  productExceptSelf(std::vector<int> &nums) override final;
};

struct Solution {
  std::vector<int> productExceptSelf(std::vector<int> &nums);
};
