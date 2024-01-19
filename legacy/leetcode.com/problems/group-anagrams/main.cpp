#include "solution.hpp"

#include <iostream>

int main() {
  std::vector<std::string> words;
  std::string n;
  while (std::cin >> n)
    words.push_back(n);
  Solution solution;
  for (std::string &n : words)
    std::cout << n << ", ";
  std::cout << std::endl;
  auto result = solution.groupAnagrams(words);
  for (auto &r : result) {
    std::cout << "> ";
    for (auto &w : r)
      std::cout << w << ", ";
    std::cout << std::endl;
  }
  return 0;
}
