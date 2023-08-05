#pragma once

#include <string>

// Problem statement: https://leetcode.com/problems/valid-parentheses

struct SolutionImpl {
  virtual bool isValid(std::string s) = 0;
};

struct StackSolution : SolutionImpl {
  virtual bool isValid(std::string s) override final;
};

struct Solution {
  bool isValid(std::string s);
};
