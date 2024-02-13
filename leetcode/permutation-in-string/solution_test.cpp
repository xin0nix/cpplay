#include <gtest/gtest.h>

#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;

class Solution {
public:
  bool checkInclusion(string_view str, string_view text) {
    unordered_map<char, int> key;
    unordered_map<char, int> window;
    if (str.size() > text.size())
      return false;
    for (auto c : str)
      key[c]++;
    for (auto c : text.substr(0, str.size()))
      window[c]++;
    for (int l = 0, r = str.size(); r < text.size(); ++l, ++r) {
      // TODO:
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