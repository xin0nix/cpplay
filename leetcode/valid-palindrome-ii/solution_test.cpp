#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using std::string;

struct Solution {
  bool validPalindrome(const string &str) {
    if (str.size() <= 1) {
      return true;
    }
    size_t l = 0;
    size_t r = str.size() - 1;
    bool missed = false;
    while (l <= r) {
      if (str[l] != str[r]) {
        if (missed) {
          return false;
        }
        missed = true;
        if (str[l + 1] == str[r]) {
          l += 1;
        } else if (str[l] == str[r - 1]) {
          r -= 1;
        } else {
          return false;
        }
      }
      l += 1;
      r -= 1;
    }
    return true;
  }
};

TEST(ValidPalindrome2, Cases) {
  {
    SCOPED_TRACE("Positive");
    EXPECT_TRUE(Solution().validPalindrome("a"));
    EXPECT_TRUE(Solution().validPalindrome("ab"));
    EXPECT_TRUE(Solution().validPalindrome("aba"));
    EXPECT_TRUE(Solution().validPalindrome("abca"));
    EXPECT_TRUE(Solution().validPalindrome("aecca"));
    EXPECT_TRUE(Solution().validPalindrome("ahehba"));
    EXPECT_TRUE(Solution().validPalindrome("bnvfqmgmlcupuufuupuculmgmqfvnb"));
  }
  {
    SCOPED_TRACE("Negative");
    EXPECT_FALSE(Solution().validPalindrome("abc"));
    EXPECT_FALSE(Solution().validPalindrome("abeccbha"));
    EXPECT_FALSE(Solution().validPalindrome("abcdfeedcbga"));
  }
}
