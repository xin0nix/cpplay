#include "solution.hpp"

#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

bool BasicSolution::isValidSudoku(vector<vector<char>> &board) {
  using cell = std::pair<int, int>;
  using affineMap = std::function<cell(const int i, const int j)>;
  affineMap toRows = [](const int i, const int j) -> cell { return {i, j}; };
  affineMap toCols = [](const int i, const int j) -> cell { return {j, i}; };
  affineMap toGrid = [](const int i, const int j) -> cell {
    int fr = ((i * 3) / 9) * 3;
    int fc = (i * 3) % 9;
    int r = fr + (j / 3);
    int c = fc + (j % 3);
    return {r, c};
  };
  struct {
    const vector<vector<char>> &board;
    bool seen[9];
    bool check(affineMap projection) {
      for (int i = 0; i < 9; ++i) {
        std::fill_n(std::begin(seen), 9, false);
        for (int j = 0; j < 9; ++j) {
          auto [r, c] = projection(i, j);
          char code = board[r][c];
          if (code == '.')
            continue;
          int numIdx = (int)(code - '0') - 1;
          if (seen[numIdx])
            return false;
          seen[numIdx] = true;
        }
      }
      return true;
    };
  } verifier{board};
  if (!verifier.check(toRows))
    return false;
  if (!verifier.check(toCols))
    return false;
  if (!verifier.check(toGrid))
    return false;
  return true;
}

bool Solution::isValidSudoku(vector<vector<char>> &board) {
  return BasicSolution().isValidSudoku(board);
}
