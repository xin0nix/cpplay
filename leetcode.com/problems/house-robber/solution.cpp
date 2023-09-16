#include "solution.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>

int BFSolution::rob(std::vector<int> &nums) {
  struct {
    const std::vector<int> &nums;
    std::map<size_t, int> memo;
    int visit(size_t start = 0) {
      if (start == nums.size())
        return 0;
      if (start == nums.size() - 1)
        return nums[start];
      auto it = memo.find(start);
      if (it != memo.end())
        return it->second;
      int p = std::max(nums[start] + visit(start + 2), visit(start + 1));
      memo[start] = p;
      return p;
    }
  } recursive{nums};
  return recursive.visit();
}

int Solution::rob(std::vector<int> &&nums) { return BFSolution().rob(nums); }
