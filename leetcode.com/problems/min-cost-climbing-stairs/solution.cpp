#include "solution.hpp"

#include <algorithm>
#include <unordered_map>

int BottomUpSolution::minCostClimbingStairs(std::vector<int> &cost) {
  // We go from right to the left, picking jump with the lower cost between the
  // two options that are currently available. As the destination is located on
  // the "top" of the staircase (i.e. outside the vector), we initialize it as
  // having 0 cost.
  // Note: the algorithm works in both directions, because we can start either
  // from the 0th, or 1th element, which is the same as making 1 or 2 jump

  int d1 = 0, d2 = 0;
  // we can either make single jump, or double from cur to l, r
  for (int c : cost) {
    // [c, .., d2], [c, d1]
    c += std::min(d2, d1);
    d2 = d1;
    d1 = c;
  }
  return std::min(d2, d1);
}

int BFSolution::minCostClimbingStairs(std::vector<int> &cost) {
  struct {
    const std::vector<int> &cost;
    std::unordered_map<size_t, int> memo;
    int visit(size_t start = 0) {
      if (start >= cost.size())
        return 0; // we can stay on the top for free
      if (start == cost.size() - 1)
        return cost[start]; // only the one option here
      auto it = memo.find(start);
      if (it != memo.end())
        return it->second;
      int q = cost[start] + std::min(visit(start + 1), visit(start + 2));
      memo[start] = q;
      return q;
    }
  } dfs{cost};
  return std::min(dfs.visit(), dfs.visit(1));
}

int Solution::minCostClimbingStairs(std::vector<int> cost) {
  return BottomUpSolution().minCostClimbingStairs(cost);
}
