#include "solution.hpp"

#include <algorithm>
#include <limits>
#include <unordered_set>

int BasicSolution::cutRod(int price[], const int n) {
  if (n == 0)
    return 0;
  const int low = std::numeric_limits<int>::min();
  struct {
    const int *price;
    std::vector<int> memo;

    inline int getPrice(int i) const { return price[i - 1]; }
    inline void memoize(int i, int q) { memo[i - 1] = q; }
    inline int recall(int i) const { return memo[i - 1]; }

    int cutRodAux(int n) {
      if (n == 0)
        return 0;
      int r = recall(n);
      if (r != low)
        return r;
      int q = low;
      for (int i = 1; i <= n; ++i)
        q = std::max(q, getPrice(i) + cutRodAux(n - i));
      memoize(n, q);
      return q;
    }

  } dp{price, std::vector<int>(n, low)};
  return dp.cutRodAux(n);
}

int BottomUpSolution::cutRod(int price[], const int n) {
  if (n == 0)
    return 0;
  std::vector<int> r(n + 1, 0);
  for (int i = 1; i <= n; ++i) {
    // inv: r[i-1] contains maximum profit from the prev step
    int q = 0;
    for (int j = 1; j <= i; ++j)
      q = std::max(q, price[j - 1] + r[i - j]);
    r[i] = q;
  }
  return r[n];
}

int Solution::cutRod(int price[], int n) {
  return BasicSolution().cutRod(price, n);
}
