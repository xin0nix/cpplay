#include "solution.hpp"

#include <iostream>

int main() {
  // clang-format off
  std::vector<std::vector<int>> grid{
      {1,2,2,3,5},
      {3,2,3,4,4},
      {2,4,5,3,1},
      {6,7,1,4,5},
      {5,1,1,2,4}
  };
  // clang-format on
  std::vector<std::vector<int>> result = Solution().pacificAtlantic(grid);
  for (auto &pt : result) {
    std::cout << pt[0] << ", " << pt[1] << "\n";
  }
  // TODO: print the results
  return 0;
}
