#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/number-of-islands/

struct SolutionImpl {
  virtual int numIslands(std::vector<std::vector<char>> &grid) = 0;
};

/**
Runtime 63 ms Beats 15.67%
Memory 19.3 MB Beats 14.9%
*/
struct SetNDequeSolution : SolutionImpl {
  virtual int numIslands(std::vector<std::vector<char>> &grid) override final;
};

/**
Runtime 28ms Beats 96.60%
Memory 12.34mb Beats 65.59%
*/
struct RewriterSolution : SolutionImpl {
  virtual int numIslands(std::vector<std::vector<char>> &grid) override final;
};

struct Solution {
  int numIslands(std::vector<std::vector<char>> &grid);
};
