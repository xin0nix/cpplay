#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/pacific-atlantic-water-flow/
struct SolutionImpl {
  virtual std::vector<std::vector<int>>
  pacificAtlantic(std::vector<std::vector<int>> &heights) = 0;
};

// Leetcode score:
// Runtime 56ms Beats 28.52 %
// Memory 21.37MB Beats 24.74 %
struct BreadthFirstSolution : SolutionImpl {
  virtual std::vector<std::vector<int>>
  pacificAtlantic(std::vector<std::vector<int>> &heights) override final;
};

// Letcode score:
// Runtime 56ms Beats 27.95 %
// Memory 25.94MB Beats 15.41 %
struct DepthFirstSolution : SolutionImpl {
  virtual std::vector<std::vector<int>>
  pacificAtlantic(std::vector<std::vector<int>> &heights) override final;
};

struct Solution {
  std::vector<std::vector<int>>
  pacificAtlantic(std::vector<std::vector<int>> &heights);
};
