#include "solution.hpp"

#include <algorithm>
#include <cctype>

bool BruteForceSolution::isPalindrome(std::string s) {
  // Filter all non alpha-num characters
  std::string filtered;
  for (char c : s)
    if (std::isalnum(c))
      filtered += std::tolower(c);
  // Reverse (a copy) and compare
  std::string reversed = filtered;
  std::reverse(reversed.begin(), reversed.end());
  return filtered == reversed;
}

bool TwoPointerSolution::isPalindrome(std::string s) {
  int l = 0;
  int r = s.size() - 1;
  while (l < r) {
    // skip non-alpha characters on both sides
    if (!std::isalnum(s[l])) {
      ++l;
      continue;
    }
    if (!std::isalnum(s[r])) {
      --r;
      continue;
    }
    if (std::tolower(s[l]) != std::tolower(s[r]))
      return false;
    ++l;
    --r;
  }
  return true;
}

bool Solution::isPalindrome(std::string s) {
  return BruteForceSolution().isPalindrome(s);
}
