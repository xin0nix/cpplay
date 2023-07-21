#include "solution.hpp"

#include <iostream>

int main() {
  std::vector<int> numbers;
  int n;
  while (std::cin.eof()) {
    std::cin >> n;
    numbers.push_back(n);
  }
  SetSolution solution;
  for (int n : numbers)
    std::cout << n << ", ";
  std::cout << std::endl;
  int result = solution.containsDuplicate(numbers);
  std::cout << "result: " << result << std::endl;
  // TODO: Print the result or do something with it.
  return 0;
}
