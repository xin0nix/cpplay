#include <cstddef>
#include <cstring>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

struct Solution {
  bool isIsomorphic(const std::string &lhs, const std::string &rhs) {
    if (lhs.size() != rhs.size()) {
      return false;
    }
    std::array<char, 128> lhsMap;
    std::array<char, 128> rhsMap;
    std::memset(lhsMap.data(), -1, lhsMap.size());
    std::memset(rhsMap.data(), -1, rhsMap.size());
    for (size_t i = 0U, e = lhs.size(); i < e; ++i) {
      char l = lhs[i];
      char r = rhs[i];
      // Each map is initialized with -1
      // If both are not initialized, we get -1 == -1
      // Otherwise they should map into each other (be isomorphic)
      if (lhsMap[l] != rhsMap[r]) {
        return false;
      }
      // Both maps contain last occurance of valid the [l,r] pair
      // So each pair has an unique distinguished index we can rely upon
      lhsMap[l] = i;
      rhsMap[r] = i;
    }
    return true;
  }
};

TEST(IsomorphicStringsTest, BasicTest) {
  Solution solution;
  EXPECT_TRUE(solution.isIsomorphic("egg", "add"));
  EXPECT_FALSE(solution.isIsomorphic("foo", "bar"));
  EXPECT_TRUE(solution.isIsomorphic("paper", "title"));
}

TEST(IsomorphicStringsTest, EdgeCasesTest) {
  Solution solution;
  EXPECT_TRUE(solution.isIsomorphic("", ""));
  EXPECT_FALSE(solution.isIsomorphic("a", "ab"));
  EXPECT_FALSE(solution.isIsomorphic("ab", "a"));
}

TEST(IsomorphicStringsTest, LongStringsTest) {
  Solution solution;
  std::string s = "abcdefghijklmnopqrstuvwxyz";
  std::string t = "abcdefghijklmnopqrstuvwxyz";
  EXPECT_TRUE(solution.isIsomorphic(s, t));
  t = "abcdefghijklmnopqrstuvwxyzabc";
  EXPECT_FALSE(solution.isIsomorphic(s, t));
}

TEST(IsomorphicStringsTest, RepeatedCharactersTest) {
  Solution solution;
  EXPECT_TRUE(solution.isIsomorphic("aa", "bb"));
  EXPECT_TRUE(solution.isIsomorphic("aaa", "bbb"));
  EXPECT_FALSE(solution.isIsomorphic("aa", "ab"));
  EXPECT_FALSE(solution.isIsomorphic("ab", "aa"));
  EXPECT_FALSE(solution.isIsomorphic("aaa", "abb"));
}

TEST(IsomorphicStringsTest, SameStringTest) {
  Solution solution;
  EXPECT_TRUE(solution.isIsomorphic("hello", "hello"));
  EXPECT_TRUE(solution.isIsomorphic("world", "world"));
}

TEST(IsomorphicStringsTest, NonAlphabeticCharactersTest) {
  Solution solution;
  EXPECT_TRUE(solution.isIsomorphic("hello!", "hell0#"));
  EXPECT_TRUE(solution.isIsomorphic("hello!", "hello?"));
  EXPECT_TRUE(solution.isIsomorphic("hello123", "hello321"));
  EXPECT_TRUE(solution.isIsomorphic("biffu%^&", "hello123"));
  EXPECT_TRUE(solution.isIsomorphic("abc123", "def456"));
  EXPECT_TRUE(solution.isIsomorphic("a1b2c3", "d4e5f6"));
  EXPECT_TRUE(solution.isIsomorphic("123abc", "456def"));
  EXPECT_FALSE(solution.isIsomorphic("xyz@#", "abc123"));
  EXPECT_FALSE(solution.isIsomorphic("hello world!", "goodbye world"));
  EXPECT_FALSE(solution.isIsomorphic("foo bar baz", "qux quux quuz"));
}