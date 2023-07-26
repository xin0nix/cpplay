#include "solution.hpp"

#include <iostream>

int main() {
  std::string s, t;
  std::cin >> s;
  std::cin >> t;
  Solution solution;
  int result = solution.isAnagram(s, t);
  std::cout << "Is anagram: " << s << ", " << t << ": " << result << std::endl;
  return 0;
}
