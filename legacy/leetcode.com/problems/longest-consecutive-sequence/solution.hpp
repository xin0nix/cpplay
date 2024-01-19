#pragma once

#include <vector>

// Problem statement:
// https://leetcode.com/problems/longest-consecutive-sequence/
using std::vector;

struct SolutionImpl {
  virtual int longestConsecutive(vector<int> &nums) = 0;
};

// Runtime: 148 ms, beats 48%
// Memory: 57 MB, beats 30%
// NOTE: came up with the solution completely on my own, which is good
// tag: #my-solution
struct BasicSolution : SolutionImpl {
  virtual int longestConsecutive(vector<int> &nums) override final;
};

struct Solution {
  int longestConsecutive(vector<int> &nums);
};
