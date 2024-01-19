#pragma once

#include <string>

// Problem statement: https://leetcode.com/problems/valid-palindrome/

struct SolutionImpl {
  virtual bool isPalindrome(std::string s) = 0;
};

// Leetcode score:
// Runtime: 3 ms, Beats 90.82%
// Memory: 7.78 mb, Beats 22.67%
struct BruteForceSolution : SolutionImpl {
  virtual bool isPalindrome(std::string s) override final;
};

// Leetcode score:
// Runtime: 4ms, Beats 71.66%
// Memory: 7.19mb, Beats 99.14%
struct TwoPointerSolution : SolutionImpl {
  virtual bool isPalindrome(std::string s) override final;
};

struct Solution {
  bool isPalindrome(std::string s);
};
