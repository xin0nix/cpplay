#include "solution.hpp"

#include <algorithm>
#include <limits>

int BruteForceSolution::maxSubArray(std::vector<int> &nums) {
  if (nums.empty())
    return 0;
  int maxSum = std::numeric_limits<int>::min();
  // compare all sums possible
  for (size_t i = 0; i < nums.size(); ++i) {
    int jSum = 0;
    for (size_t j = i; j < nums.size(); ++j) {
      jSum += nums[j];
      maxSum = std::max(jSum, maxSum);
    }
  }
  return maxSum;
}

int findMaxSubarray(const std::vector<int> &nums, size_t low, size_t high) {
  if (high - low == 1)
    return nums[low];

  // [from..->..to) +1
  // (to..<-..from] -1
  auto findMaxCrossing = [&nums](size_t from, size_t to, int step) {
    int maxSum = std::numeric_limits<int>::min();
    int sum = 0;
    for (size_t i = from; i != to; i += step) {
      sum += nums[i];
      maxSum = std::max(sum, maxSum);
    }
    return maxSum;
  };

  size_t mid = (low + high) / 2;
  return std::max(
      {findMaxSubarray(nums, low, mid), findMaxSubarray(nums, mid, high),
       // [low..<-..mid) + [mid..->..high)
       findMaxCrossing(mid - 1, low - 1, -1) + findMaxCrossing(mid, high, 1)});
}

int DivideConquerSolution::maxSubArray(std::vector<int> &nums) {
  if (nums.empty())
    return 0;
  return findMaxSubarray(nums, 0, nums.size());
}

int SlidingWindowSolution::maxSubArray(std::vector<int> &nums) {
  if (nums.empty())
    return 0;
  int maxSum = std::numeric_limits<int>::min();
  int sum = 0;
  for (int n : nums) {
    sum += n;
    maxSum = std::max(sum, maxSum);
    if (sum < 0)
      sum = 0;
  }
  return maxSum;
}

int Solution::maxSubArray(std::vector<int> &nums) {
  return DivideConquerSolution().maxSubArray(nums);
}
