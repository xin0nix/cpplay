#include "solution.hpp"

#include <algorithm>

int BottomUpSolution::minCostClimbingStairs(std::vector<int> &cost) {
  // We go from right to the left, picking jump with the lower cost between the
  // two options that are currently available. As the destination is located on
  // the "top" of the staircase (i.e. outside the vector), we initialize it as
  // having 0 cost.
  // Note: the algorithm works in both directions, because we can start either
  // from the 0th, or 1th element, which is the same as making 1 or 2 jump

  int a = cost.front(), b = 0;
  for (size_t i = 1; i != cost.size(); ++i) {
    int c = cost[i];
    // we can either make single jump, or double from cur to a / b
    c = std::min(c + a, c + b);
    std::swap(a, b);
    a = c;
  }
  return std::min(a, b);
}

int Solution::minCostClimbingStairs(std::vector<int> cost) {
  return BottomUpSolution().minCostClimbingStairs(cost);
}
