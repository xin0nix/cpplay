#include "solution.hpp"

#include <iostream>

int main() {
  std::vector<int> numbers;
  int k;
  std::cin >> k;
  int n;
  while (std::cin >> n) {
    numbers.push_back(n);
  }
  Solution solution;
  for (int n : numbers)
    std::cout << n << ", ";
  std::cout << std::endl;
  auto result = solution.topKFrequent(numbers, k);
  for (int r : result)
    std::cout << r << ", ";
  std::cout << std::endl;
  // TODO: Print the result or do something with it.
  return 0;
}
