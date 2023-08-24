#include "solution.hpp"

int GreedySolution::jump(std::vector<int> &nums) {
  int level = 0;
  int lastIdx = nums.size() - 1;
  for (int l = 0, r = 0, farest = 0; r < lastIdx; ++level) {
    for (int i = l; i <= r; ++i)
      farest = std::max(farest, i + nums[i]);
    l = r + 1;
    r = farest;
  }
  return level;
}

int Solution::jump(std::vector<int> &nums) {
  return GreedySolution().jump(nums);
}
