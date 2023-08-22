#include "solution.hpp"

#include <deque>
#include <stack>
#include <unordered_set>

bool GreedySolution::canJump(std::vector<int> &nums) {
  int goal = nums.size() - 1;
  for (int ri = goal; ri >= 0; --ri) {
    int jump = nums[ri];
    if (ri + jump >= goal)
      goal = ri;
  }
  return goal == 0;
}

bool DPSolution::canJump(std::vector<int> &nums) {
  if (nums.empty())
    return false;
  std::unordered_set<int> seen;
  std::stack<int> q;
  q.push(0);
  while (!q.empty()) {
    int idx = q.top();
    if (idx == nums.size() - 1)
      return true; // we've reached the end!
    q.pop();
    if (seen.count(idx))
      continue;
    int jumps = nums[idx];
    for (int i = 1; i <= jumps; ++i)
      if (idx + i < nums.size())
        q.push(idx + i);
  }
  return false;
}

bool Solution::canJump(std::vector<int> &nums) {
  return DPSolution().canJump(nums);
}
