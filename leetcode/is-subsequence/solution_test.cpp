#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

using std::string;

struct Solution {
  bool isSubsequence(string subSeq, string text) {
    auto s = subSeq.cbegin();
    auto t = text.cbegin();
    while (s != subSeq.cend() and t != text.cend()) {
      if (*s == *t) {
        ++s;
      }
      ++t;
    }
    return s == subSeq.cend();
  }
};

TEST(SubSeqTest, Leet) {
  EXPECT_TRUE(Solution().isSubsequence("abc", "ahbgdc"));
  EXPECT_FALSE(Solution().isSubsequence("axc", "ahbgdc"));
}

TEST(SubSeqTest, CornereCases) {
  EXPECT_FALSE(Solution().isSubsequence("abc", ""));
  EXPECT_FALSE(Solution().isSubsequence("abc", "ab"));
  EXPECT_TRUE(Solution().isSubsequence("", "abc"));
  EXPECT_TRUE(Solution().isSubsequence("ab", "abc"));
}
