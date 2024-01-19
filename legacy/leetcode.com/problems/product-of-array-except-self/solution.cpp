#include "solution.hpp"

#include <algorithm>
#include <unordered_set>

std::vector<int> BasicSolution::productExceptSelf(std::vector<int> &nums) {
  std::vector<int> res(nums.size(), 1);
  // res will store prefixes, prefix of the 0th element is 1
  for (int i = 1; i < nums.size(); ++i)
    res[i] = res[i - 1] * nums[i - 1];
  int suffix = 1;
  // now go in reverse and multiply prefixes by the suffix
  for (int i = nums.size() - 1; i >= 0; --i) {
    res[i] *= suffix;
    suffix *= nums[i];
  }
  return res;
}

std::vector<int> Solution::productExceptSelf(std::vector<int> &nums) {
  return BasicSolution().productExceptSelf(nums);
}
