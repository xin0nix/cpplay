#pragma once

#include <string>
#include <vector>

using namespace std;

// Problem statement: https://leetcode.com/problems/word-search/

struct SolutionImpl {
  virtual bool exist(vector<vector<char>> &board, string word) = 0;
};

// Solved on the first try without any help
// Runtime: 270 ms, beats 90%
// Memory: 8.33 MB, beats 13%
struct BasicSolution : SolutionImpl {
  virtual bool exist(vector<vector<char>> &board, string word) override final;
};

struct Solution {
  bool exist(vector<vector<char>> &board, string word);
};
