#include "solution.hpp"

#include <iostream>

int main() {
  // clang-format off
  std::vector<std::vector<int>> grid{
      {1 ,  2,  3,  4,  5},
      {16, 17, 18, 19,  6},
      {15, 24, 25, 20,  7},
      {14, 23, 22, 21,  8},
      {13, 12, 11, 10,  9}
  };
  // clang-format on
  std::vector<std::vector<int>> result = Solution().pacificAtlantic(grid);
  for (auto &pt : result) {
    std::cout << pt[0] << ", " << pt[1] << "\n";
  }
  // TODO: print the results
  return 0;
}
