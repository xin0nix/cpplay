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
  auto result = solution.productExceptSelf(numbers);
  std::cout << "result: \n";
  for (int r : result)
    std::cout << r << ", ";
  std::cout << std::endl;
  return 0;
}
