#include "solution.hpp"

#include <algorithm>
#include <unordered_set>

bool SetSolution::containsDuplicate(std::vector<int> &nums) {
  std::unordered_set<int> seen;
  for (int n : nums)
    if (!seen.insert(n).second)
      return true;
  return false;
}

bool SortSolution::containsDuplicate(std::vector<int> &nums) {
  std::sort(nums.begin(), nums.end());
  for (int i = 1; i < nums.size(); ++i)
    if (nums[i] == nums[i - 1])
      return true;
  return false;
}

bool Solution::containsDuplicate(std::vector<int> &nums) {
  return SortSolution().containsDuplicate(nums);
}
