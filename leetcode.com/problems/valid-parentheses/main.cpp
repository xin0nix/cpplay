#include "solution.hpp"

#include <iostream>

int main() {
  std::string str;
  std::cin >> str;
  Solution solution;
  bool result = solution.isValid(str);
  std::cout << "result: " << result << std::endl;
  return 0;
}
