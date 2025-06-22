#include <gtest/gtest.h>
#include <string>
#include <vector>

class Solution {
public:
  bool wordBreak(std::string_view str,
                 const std::vector<std::string> &wordDict) {
    // Initialize dynamic programming table
    int strLen = int(str.size());
    std::vector<bool> dyn(strLen + 1, false);
    dyn.at(strLen) = true;
    // Fill the table top-bottom per say
    for (int pos = strLen - 1; pos >= 0; --pos) {
      int count = strLen - pos;
      auto substr = str.substr(pos, count);
      for (std::string_view word : wordDict) {
        if (substr.starts_with(word)) {
          std::string_view match = word;
          if (dyn.at(pos + match.length())) {
            dyn.at(pos) = true;
          }
        }
      }
    }
    return dyn.at(0);
  }
};

// Struct to hold each test case's parameters
struct WordBreakTestCase {
  std::string s;
  std::vector<std::string> wordDict;
  bool expected;
};

// Parameterized test fixture
class WordBreakTest : public ::testing::TestWithParam<WordBreakTestCase> {};

TEST_P(WordBreakTest, HandlesVariousCases) {
  const auto &param = GetParam();
  Solution sol;
  EXPECT_EQ(sol.wordBreak(param.s, param.wordDict), param.expected);
}

// List of test cases
INSTANTIATE_TEST_SUITE_P(
    WordBreakTests, WordBreakTest,
    ::testing::Values(
        // Bug0
        WordBreakTestCase{"cars", {"car", "ca", "rs"}, true},
        // Basic case: can be segmented
        WordBreakTestCase{"leetcode", {"leet", "code"}, true},
        // Basic case: cannot be segmented
        WordBreakTestCase{
            "catsandog", {"cats", "dog", "sand", "and", "cat"}, false},
        // Multiple ways to segment
        WordBreakTestCase{"applepenapple", {"apple", "pen"}, true},
        // Empty string
        WordBreakTestCase{"", {"a", "b"}, true},
        // String with one character, in dict
        WordBreakTestCase{"a", {"a"}, true},
        // String with one character, not in dict
        WordBreakTestCase{"b", {"a"}, false},
        // WordDict has empty string
        WordBreakTestCase{"a", {""}, false},
        // Repeated words in dict
        WordBreakTestCase{"aaaaaaa", {"aaaa", "aaa"}, true},
        // Large input, cannot be segmented
        WordBreakTestCase{
            "aaaaaaaaaaaaaaaaaaaaaab", {"a", "aa", "aaa", "aaaa"}, false}));
