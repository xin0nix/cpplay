#include <gtest/gtest.h>
#include <string>
using namespace std;

struct Solution {
  using IterType = decltype((declval<std::string>()).cbegin());
  auto check(IterType begin, IterType end, IterType left, IterType right) {
    while (left >= begin and right < end and *left == *right) {
      left--;
      right++;
    }
    return std::make_tuple(left + 1, right);
  }

  string longestPalindrome(const std::string &str) {
    IterType begin = str.cbegin();
    IterType end = str.cend();
    IterType resLeft;
    IterType resRight;
    int maxLen = 0;
    auto update = [&](auto &interval) {
      auto &&[left, right] = interval;
      auto dist = std::distance(left, right);
      if (dist > maxLen) {
        resLeft = left;
        resRight = right;
        maxLen = dist;
      }
    };
    for (auto cur = begin; cur != end; ++cur) {
      auto odd = check(begin, end, cur, cur);
      update(odd);
      auto even = check(begin, end, cur, cur + 1);
      update(even);
    }
    return {resLeft, resRight};
  }
};

struct TestCase {
  string input;
  string expected;
};

class LongestPalindromeTest : public ::testing::TestWithParam<TestCase> {};

TEST_P(LongestPalindromeTest, HandlesVariousInputs) {
  Solution sol;
  TestCase tCase = GetParam();
  string result = sol.longestPalindrome(tCase.input);
  EXPECT_EQ(result, tCase.expected);
}

INSTANTIATE_TEST_SUITE_P(
    LongestPalindromeTests, LongestPalindromeTest,
    ::testing::Values(
        TestCase{"babad", "bab"}, // "bab" or "aba" both valid, length 3
        TestCase{"cbbd", "bb"},   // "bb" length 2
        TestCase{"a", "a"},       // single char
        TestCase{"ac", "a"},      // "a" or "c"
        TestCase{"", ""},         // empty string
        TestCase{"forgeeksskeegfor", "geeksskeeg"}, // length 10 palindrome
        TestCase{"abacdfgdcaba", "aba"}, // multiple palindromes of length 3
        TestCase{"abacdedcaba", "abacdedcaba"} // entire string palindrome
        ));
