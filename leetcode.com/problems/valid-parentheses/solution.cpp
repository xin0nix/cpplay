#include "solution.hpp"

#include <algorithm>
#include <stack>

bool StackSolution::isValid(std::string str) {
  std::stack<char> seen;
  for (char c : str) {
    if (c == '(' || c == '[' || c == '{') {
      seen.push(c);
      continue;
    }
    if (seen.empty())
      return false;
    char top = seen.top();
    if ((c == ')' && top == '(') || (c == ']' && top == '[') ||
        (c == '}' && top == '{')) {
      seen.pop();
      continue;
    }
    return false;
  }
  return seen.empty();
}

bool Solution::isValid(std::string s) { return StackSolution().isValid(s); }
