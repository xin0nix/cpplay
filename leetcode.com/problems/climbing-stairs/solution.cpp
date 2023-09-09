#include "solution.hpp"

#include <iostream>
#include <unordered_set>

int BasicSolution::climbStairs(int n) {
  if (n == 0)
    return 0;
  struct {
    const int n;
    std::vector<int> memo;
    int visit(int i) {
      if (i > n)
        return 0;
      if (i == n)
        return 1;
      if (memo[i] != -1)
        return memo[i];
      int q = visit(i + 1) + visit(i + 2);
      memo[i] = q;
      return q;
    }
  } dfs{n, std::vector<int>(n, -1)};
  return dfs.visit(0);
}

int BottomUpSolution::climbStairs(int n) {
  if (n == 0)
    return 0;
  int first = 1;
  int second = 1;
  for (int i = 0; i < n - 1; ++i) {
    int tmp = first;
    first += second;
    second = tmp;
  }
  return first;
}

int Solution::climbStairs(int n) { return BasicSolution().climbStairs(n); }
