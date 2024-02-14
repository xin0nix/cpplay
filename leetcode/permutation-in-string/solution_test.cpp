#include <gtest/gtest.h>

#include <algorithm>
#include <fmt/core.h>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;

class Solution {
public:
  bool checkInclusion(string_view target, string_view message) {
    if (target.size() > message.size())
      return false;
    unordered_map<char, int> key;
    unordered_map<char, int> window;
    for (auto c : target)
      key[c]++;
    for (auto c : message.substr(0, target.size()))
      window[c]++;
    for (int l = 0, r = target.size(); r <= message.size(); ++l, ++r) {
      if (l > 0) {
        // [l, r)
        char cOut = message[l - 1];
        char cIn = message[r - 1];
        window[cOut] -= 1;
        window[cIn] += 1;
      }
      bool mismatch =
          (ranges::find_if_not(key.begin(), key.end(), [&](auto &p) -> bool {
             // do elements match ?
             auto &[k, v] = p;
             if (window[k] == v)
               return true;
             return false;
           }) != key.end());
      if (mismatch)
        continue;
      return true;
    }
    return false;
  }
};

TEST(PermutationInString, LeetCodeExample1) {
  ASSERT_TRUE(Solution().checkInclusion("ab", "eidbaooo"));
}

TEST(PermutationInString, LeetCodeExample2) {
  ASSERT_FALSE(Solution().checkInclusion("ab", "eidboaoo"));
}

TEST(PermutationInString, LeetCodeBug1) {
  ASSERT_TRUE(Solution().checkInclusion("adc", "dcda"));
}