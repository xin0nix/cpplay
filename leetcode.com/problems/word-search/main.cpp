#include "solution.hpp"

#include <iostream>

int main() {
  // clang-format off
  vector<vector<char>> board{
    {'A','B','C','E'},
    {'S','F','C','S'},
    {'A','D','E','E'}
  };
  // clang-format on
  string word = "ABCB";
  bool result = Solution().exist(board, word);
  std::cout << "result: " << result << std::endl;
  return 0;
}
