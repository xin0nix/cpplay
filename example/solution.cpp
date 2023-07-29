#include "solution.hpp"

#include <algorithm>
#include <unordered_set>

bool BasicSolution::solve(std::vector<int> &nums) {
  std::unordered_set<int> seen;
  for (int n : nums)
    if (!seen.insert(n).second)
      return true;
  return false;
}

bool Solution::solve(std::vector<int> &nums) {
  return BasicSolution().solve(nums);
}
