#include "solution.hpp"

#include <iostream>

int main() {
  std::string s;
  std::cin >> s;
  std::cout << "result: " << Solution().lengthOfLongestSubstring(s)
            << std::endl;
  return 0;
}
