#include "solution.hpp"

#include <iostream>

int main() {
  Solution solution;
  std::vector<int> numbers;
  int n;
  while (std::cin >> n)
    numbers.push_back(n);
  solution.sort(numbers);
  for (int n : numbers)
    std::cout << n << ", ";
  std::cout << std::endl;
  return 0;
}
