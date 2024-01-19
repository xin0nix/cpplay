#pragma once

#include <string>
#include <vector>

// Problem statement: https://leetcode.com/problems/generate-parentheses/

struct SolutionImpl {
  virtual std::vector<std::string> generateParenthesis(int n) = 0;
};

struct BactrackingSolution : SolutionImpl {
  virtual std::vector<std::string> generateParenthesis(int n) override final;
};

struct Solution {
  std::vector<std::string> generateParenthesis(int n);
};
