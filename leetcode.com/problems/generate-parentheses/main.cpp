#include "solution.hpp"

#include <iostream>

int main() {
  std::vector<int> numbers;
  int n;
  std::cin >> n;
  Solution solution;
  auto result = solution.generateParenthesis(n);
  std::cout << "result: \n";
  for (auto &s : result)
    std::cout << s << ", ";
  std::cout << std::endl;
  return 0;
}
