#include "solution.hpp"

#include <algorithm>

std::vector<std::vector<int>> BasicSolution::subsets(std::vector<int> &nums) {
  if (nums.empty())
    return {};
  struct {
    const std::vector<int> &nums;
    std::vector<std::vector<int>> res;
    void traverse(size_t i, std::vector<int> s) {
      if (i == nums.size()) {
        res.push_back(std::move(s));
        return;
      }
      traverse(i + 1, s);
      s.push_back(nums[i]);
      traverse(i + 1, std::move(s));
    };
  } v{nums, {}};
  v.traverse(0, {});
  return v.res;
}

std::vector<std::vector<int>> Solution::subsets(std::vector<int> &nums) {
  return BasicSolution().subsets(nums);
}
