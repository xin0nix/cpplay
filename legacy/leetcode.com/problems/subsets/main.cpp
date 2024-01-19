#include "solution.hpp"

#include <iostream>

int main() {
  std::vector<int> numbers;
  int n;
  while (std::cin >> n)
    numbers.push_back(n);
  Solution solution;
  for (int n : numbers)
    std::cout << n << ", ";
  std::cout << std::endl;
  auto result = solution.subsets(numbers);
  std::cout << "result: " << result.size() << std::endl;
  // TODO: Print the result or do something with it.
  return 0;
}
