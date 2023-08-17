#include "solution.hpp"

#include <algorithm>

using namespace std;

int BasicSolution::lengthOfLongestSubstring(std::string s) {
  int l = 0;
  bool seen[256];
  int res = 0;
  std::fill(std::begin(seen), std::begin(seen) + 256, false);
  for (int r = 0; r < s.size(); ++r) {
    int rpos = int(s[r]);
    // skip all the elements befor the one we have already seen (if any)
    while (seen[rpos]) {
      int lpos = int(s[l]);
      seen[lpos] = false;
      l += 1;
    }
    seen[rpos] = true;
    res = std::max(res, r - l + 1);
  }
  return res;
}

int Solution::lengthOfLongestSubstring(std::string s) {
  return BasicSolution().lengthOfLongestSubstring(s);
}
