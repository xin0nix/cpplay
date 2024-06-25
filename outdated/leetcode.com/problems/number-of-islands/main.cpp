#include "solution.hpp"

#include <iostream>

int main() {
  // std::vector<std::vector<char>> grid;
  // int rows, cols;
  // std::cin >> rows >> cols;
  // for (int i = 0; i < rows; ++i) {
  //   std::vector<char> numbers;
  //   char n;
  //   for (int j = 0; j < cols; ++j) {
  //     numbers.push_back(n);
  //   }
  //   grid.push_back(std::move(numbers));
  // }
  // clang-format off
  std::vector<std::vector<char>> grid{
      {'1', '1', '1'},
      {'0', '1', '0'},
      {'1', '1', '1'},
  };
  // clang-format on
  Solution solution;
  auto result = solution.numIslands(grid);
  std::cout << result << std::endl;
  return 0;
}
