#include "solution.hpp"

#include <iostream>

int main() {
  std::vector<int> numbers;
  int n;
  while (std::cin >> n) {
    numbers.push_back(n);
  }
  Solution solution;
  for (int n : numbers)
    std::cout << n << ", ";
  std::cout << std::endl;
  bool result = solution.canJump(numbers);
  std::cout << "Can jump? : " << result << std::endl;
  return 0;
}
