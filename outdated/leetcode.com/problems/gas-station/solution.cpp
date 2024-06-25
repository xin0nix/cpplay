#include "solution.hpp"

// IMHO the solution below is better than one from the neetcode (not leetcode),
// the reason is that it more intuitive while providing the same time complexity

// gas_ = [  1,  2,  3,  4,  5]
// cost = [  3,  4,  5,  1,  2]
// diff = [ -2, -2, -2,  3,  3] // 0 in total, fine
//
// l                  = 3
// r                  = 4
// cur sum [l..r]     = 3+3 = 6
// prefix sum [0..l)  = -2+(-2)+(-2) = -6
// cur >= prefix, ok
int GreedySolution::canCompleteCircuit(std::vector<int> &gas,
                                       std::vector<int> &cost) {
  const int N = gas.size();
  if (!N)
    return -1;
  int l = 0, curSum = 0, preSum = 0;
  for (int r = 0; r < N; ++r) {
    curSum += gas[r] - cost[r];
    // we reached the end with enough gas:
    if (r == N - 1 && curSum + preSum >= 0)
      return l;
    if (curSum <= 0) {
      // Local fail
      preSum += curSum;
      curSum = 0;
      l = r + 1;
    }
  }
  return -1;
}

int Solution::canCompleteCircuit(std::vector<int> &gas,
                                 std::vector<int> &cost) {
  return GreedySolution().canCompleteCircuit(gas, cost);
}
