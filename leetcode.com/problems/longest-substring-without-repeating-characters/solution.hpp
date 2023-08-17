#pragma once

#include <string>

// Problem statement:
// https://leetcode.com/problems/longest-substring-without-repeating-characters/

struct SolutionImpl {
  virtual int lengthOfLongestSubstring(std::string s) = 0;
};

// Leetcode score:
// 4ms Beats 95.17 %
// 6.83mb Beats 96.90 %
struct BasicSolution : SolutionImpl {
  virtual int lengthOfLongestSubstring(std::string s) override final;
};

struct Solution {
  int lengthOfLongestSubstring(std::string s);
};
