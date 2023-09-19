#include "solution.hpp"

int BottomUpSolution::rob(std::vector<int> &nums) {
  if (nums.size() == 1)
    return nums.front();
  auto helper = [](auto begin, auto end) -> int {
    int d2 = 0, d1 = 0;
    for (auto it = begin; it != end; ++it) {
      // [n, .., d2] or [.., d1]
      int n = *it;
      int best = std::max(n + d2, d1);
      d2 = d1;
      d1 = best;
    }
    return d1;
  };
  return std::max(helper(++nums.begin(), nums.end()),
                  helper(++nums.rbegin(), nums.rend()));
}

int Solution::rob(std::vector<int> &&nums) {
  return BottomUpSolution().rob(nums);
}
