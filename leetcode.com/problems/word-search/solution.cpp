#include "solution.hpp"

#include <algorithm>
#include <unordered_set>

bool BasicSolution::exist(vector<vector<char>> &board, string word) {
  struct Solver {
    const vector<vector<char>> &board;
    const string &word;
    const int ROWS, COLS, LEN;
    vector<vector<bool>> seen{};
    bool visit(int r, int c, int level) {
      // out of bounds -> skip
      if (r < 0 || r == ROWS || c < 0 || c == COLS)
        return false;
      // no match -> skip
      if (board[r][c] != word[level])
        return false;
      if (seen[r][c])
        return false;
      // match the final letter -> success!
      if (level == LEN - 1)
        return true;
      seen[r][c] = true;
      level++;
      if (visit(r - 1, c, level))
        return true;
      if (visit(r + 1, c, level))
        return true;
      if (visit(r, c - 1, level))
        return true;
      if (visit(r, c + 1, level))
        return true;
      seen[r][c] = false;
      return false;
    }

  } s{board, word, (int)board.size(), (int)board[0].size(), (int)word.size()};
  s.seen = vector<vector<bool>>(s.ROWS, vector<bool>(s.COLS, false));

  for (int r = 0; r < s.ROWS; ++r)
    for (int c = 0; c < s.COLS; ++c)
      if (s.visit(r, c, 0))
        return true;
  return false;
}

bool Solution::exist(vector<vector<char>> &board, string word) {
  return BasicSolution().exist(board, word);
}
