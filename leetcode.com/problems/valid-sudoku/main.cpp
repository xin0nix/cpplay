#include "solution.hpp"

#include <iostream>

int main() {
  // clang-format off
  vector<vector<char>> board{
     {'8','3','.','.','7','.','.','.','.'}
    ,{'6','.','.','1','9','5','.','.','.'}
    ,{'.','9','8','.','.','.','.','6','.'}
    ,{'8','.','.','.','6','.','.','.','3'}
    ,{'4','.','.','8','.','3','.','.','1'}
    ,{'7','.','.','.','2','.','.','.','6'}
    ,{'.','6','.','.','.','.','2','8','.'}
    ,{'.','.','.','4','1','9','.','.','5'}
    ,{'.','.','.','.','8','.','.','7','9'}
  };
  // clang-format on
  Solution solution;
  bool result = solution.isValidSudoku(board);
  std::cout << "result: " << result << std::endl;
  return 0;
}
