#include "solution.hpp"

#include <iostream>

int main() {
  std::vector<int> numbers;
  numbers = {-1, -2, 0, 1, 2};
  Solution solution;
  int result = solution.longestConsecutive(numbers);
  std::cout << "result: " << result << std::endl;
  return 0;
}
