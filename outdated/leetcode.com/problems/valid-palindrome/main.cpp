#include "solution.hpp"

#include <iostream>

int main() {
  std::string phrase;
  std::cin >> phrase;
  Solution solution;
  std::cout << std::endl;
  auto result = solution.isPalindrome(phrase);
  std::cout << "result: " << result << std::endl;
  return 0;
}
