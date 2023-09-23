#pragma once

#include <vector>

using std::vector;

// Problem statement: https://leetcode.com/problems/valid-sudoku/

struct SolutionImpl {
  virtual bool isValidSudoku(vector<vector<char>> &board) = 0;
};

// Runtime: 7 ms, beats 99%
// Memory: 18 MB, beats 70%
struct BasicSolution : SolutionImpl {
  virtual bool isValidSudoku(vector<vector<char>> &board) override final;
};

struct Solution {
  bool isValidSudoku(vector<vector<char>> &board);
};
