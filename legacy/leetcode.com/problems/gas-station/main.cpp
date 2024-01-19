#include "solution.hpp"

#include <iostream>

int main() {
  std::vector<int> gas{3, 1, 1};
  std::vector<int> cost{1, 2, 2};
  // 2, -1, -1
  // 2,  1,  0
  int result = Solution().canCompleteCircuit(gas, cost);
  std::cout << "Result: " << result << "\n";
  return 0;
}
