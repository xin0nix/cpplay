#pragma once

#include <vector>

using std::vector;

// Problem statement: https://leetcode.com/problems/valid-sudoku/

struct SolutionImpl {
  virtual bool isValidSudoku(vector<vector<char>> &board) = 0;
};

// Runtime: 7 ms, beats 99%
// Memory: 18 MB, beats 70%
// NOTE: came up with the solution completely on my own, which is good
// tag: #my-solution
struct BasicSolution : SolutionImpl {
  virtual bool isValidSudoku(vector<vector<char>> &board) override final;
};

struct Solution {
  bool isValidSudoku(vector<vector<char>> &board);
};
