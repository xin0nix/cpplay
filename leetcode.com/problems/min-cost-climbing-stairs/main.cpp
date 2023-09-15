#include "solution.hpp"

#include <iostream>

int main() {
  int q =
      Solution().minCostClimbingStairs({1, 100, 1, 1, 1, 100, 1, 1, 100, 1});
  std::cout << "Res: " << q << std::endl;
  return 0;
}
