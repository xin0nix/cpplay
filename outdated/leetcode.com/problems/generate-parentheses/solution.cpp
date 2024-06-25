#include "solution.hpp"

#include <algorithm>
#include <stack>

std::vector<std::string> BactrackingSolution::generateParenthesis(int n) {
  struct {
    const int n;
    std::vector<std::string> result;
    std::vector<char> cur;
    void visit(int open = 0, int close = 0) {
      if (open == n && close == n)
        return (void)result.emplace_back(cur.begin(), cur.end());
      // We can only ')' with a preceding '('
      if (open > close) {
        cur.push_back(')');
        visit(open, close + 1);
        cur.pop_back();
      }
      // We can always add '(', with respect to the open counter
      if (open < n) {
        cur.push_back('(');
        visit(open + 1, close);
        cur.pop_back();
      }
    }
  } visitor{n};
  visitor.visit(0, 0);
  return visitor.result;
}

std::vector<std::string> Solution::generateParenthesis(int n) {
  return BactrackingSolution().generateParenthesis(n);
}
