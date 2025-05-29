#include <gtest/gtest.h>
#include <string>

struct Solution {
  using IterType = decltype(std::declval<std::string>().cbegin());

  int count(IterType begin, IterType end, IterType left, IterType right) {
    int counter = 0;
    for (; left >= begin and right < end and *left == *right; --left, ++right) {
      ++counter;
    }
    return counter;
  }

  int countSubstrings(const std::string &str) {
    int counter = 0;
    IterType begin = str.cbegin();
    IterType end = str.cend();
    for (auto middle = str.cbegin(); middle != end; ++middle) {
      counter += count(begin, end, middle, middle) +
                 count(begin, end, middle, middle + 1);
    }
    return counter;
  }
};

struct TestCase {
  std::string input;
  int expected;
};

class CountPalindromicSubstringsTest : public testing::TestWithParam<TestCase> {
};

TEST_P(CountPalindromicSubstringsTest, Test) {
  auto param = GetParam();
  Solution sol;
  int result = sol.countSubstrings(param.input);
  EXPECT_EQ(result, param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    Default, CountPalindromicSubstringsTest,
    testing::Values(TestCase{"a", 1}, TestCase{"abc", 3}, TestCase{"aaa", 6},
                    TestCase{"ababa", 9}, TestCase{"", 0}, TestCase{"x", 1},
                    TestCase{"racecar", 10}, TestCase{"aabaa", 9}));
