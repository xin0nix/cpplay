#include "solution.hpp"

#include <unordered_set>

bool SetSolution::containsDuplicate(std::vector<int> &nums) {
  // TODO: Implement your solution here.
  std::unordered_set<int> seen;
  for (int n : nums)
    if (!seen.insert(n).second)
      return true;
  return false;
}
