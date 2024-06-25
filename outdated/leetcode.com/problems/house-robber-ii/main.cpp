#include "solution.hpp"

#include <iostream>

int main() {
  std::vector<int> numbers{2, 7, 9, 3, 1};
  int result = Solution().rob(std::move(numbers));
  std::cout << "result: " << result << std::endl;
  return 0;
}
