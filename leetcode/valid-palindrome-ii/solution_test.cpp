#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using std::string;

struct Solution {
  [[nodiscard]] bool isPalindrome(const string &str, size_t l,
                                  size_t r) const noexcept {
    while (l <= r) {
      if (str[l] != str[r]) {
        return false;
      }
      l += 1;
      r -= 1;
    }
    return true;
  }

  bool validPalindrome(const string &str) {
    if (str.size() <= 1) {
      return true;
    }
    size_t l = 0;
    size_t r = str.size() - 1;
    while (l <= r) {
      if (str[l] != str[r]) {
        return isPalindrome(str, l + 1, r) || isPalindrome(str, l, r - 1);
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
    EXPECT_TRUE(Solution().validPalindrome("cupuufuupuc"));
    EXPECT_TRUE(Solution().validPalindrome("ucupuufuupuc"));
    EXPECT_TRUE(Solution().validPalindrome("cupuufuupucu"));
  }
  {
    SCOPED_TRACE("Negative");
    EXPECT_FALSE(Solution().validPalindrome("abc"));
    EXPECT_FALSE(Solution().validPalindrome("abeccbha"));
    EXPECT_FALSE(Solution().validPalindrome("abcdfeedcbga"));
  }
}
