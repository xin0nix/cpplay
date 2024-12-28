#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

using std::string;

struct BruteForce {
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
  EXPECT_TRUE(BruteForce().isSubsequence("abc", "ahbgdc"));
  EXPECT_FALSE(BruteForce().isSubsequence("axc", "ahbgdc"));
}

TEST(SubSeqTest, CornereCases) {
  EXPECT_FALSE(BruteForce().isSubsequence("abc", ""));
  EXPECT_FALSE(BruteForce().isSubsequence("abc", "ab"));
  EXPECT_TRUE(BruteForce().isSubsequence("", "abc"));
  EXPECT_TRUE(BruteForce().isSubsequence("ab", "abc"));
}
