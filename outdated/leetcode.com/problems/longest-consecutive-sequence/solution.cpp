#include "solution.hpp"

#include <algorithm>
#include <unordered_map>

using std::unordered_map;

int BasicSolution::longestConsecutive(vector<int> &nums) {
  unordered_map<int, int> seen;
  int maxLen = 0;
  for (int n : nums) {
    if (seen.count(n))
      continue;
    int l = n, r = n;
    if (seen.count(n - 1))
      l = n - seen[n - 1];
    if (seen.count(n + 1))
      r = n + seen[n + 1];
    int len = r - l + 1;
    seen[n] = len;
    seen[l] = len;
    seen[r] = len;
    maxLen = std::max(len, maxLen);
  }
  return maxLen;
}

int Solution::longestConsecutive(vector<int> &nums) {
  return BasicSolution().longestConsecutive(nums);
}
