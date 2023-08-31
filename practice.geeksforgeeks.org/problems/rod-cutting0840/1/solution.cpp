#include "solution.hpp"

#include <algorithm>
#include <limits>
#include <unordered_set>

int BasicSolution::cutRod(int price[], const int n) {
  if (n == 0)
    return 0;
  const int low = std::numeric_limits<int>::min();
  struct {
    std::vector<int> price;
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

  } dp{std::vector<int>(price, price + n), std::vector<int>(n, low)};
  return dp.cutRodAux(n);
}

int Solution::cutRod(int price[], int n) {
  return BasicSolution().cutRod(price, n);
}
