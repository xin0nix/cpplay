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
  int result = solution.maxSubArray(numbers);
  std::cout << "result: " << result << std::endl;
  // TODO: Print the result or do something with it.
  return 0;
}
