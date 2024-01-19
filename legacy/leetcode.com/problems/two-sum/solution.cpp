#include "solution.hpp"

#include <unordered_map>

std::vector<int> MapSolution::twoSum(std::vector<int> &nums, int target) {
  std::unordered_map<int, size_t> seen;
  for (size_t i = 0; i < nums.size(); ++i) {
    int n = nums[i];
    auto twin = seen.find(target - n);
    if (twin != seen.end())
      return {(int)twin->second, (int)i};
    seen[n] = i;
  }
  return {};
}

std::vector<int> Solution::twoSum(std::vector<int> &nums, int target) {
  return MapSolution().twoSum(nums, target);
}
