#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/climbing-stairs/

struct SolutionImpl {
  virtual int climbStairs(int n) = 0;
};

// 2ms, beats 44%
// 6.2 MB, beats 43%
struct BasicSolution : SolutionImpl {
  virtual int climbStairs(int n) override final;
};

// 0ms, beats 100%
// 5.9 MB, beats 77%
struct BottomUpSolution : SolutionImpl {
  virtual int climbStairs(int n) override final;
};

struct Solution {
  int climbStairs(int n);
};
