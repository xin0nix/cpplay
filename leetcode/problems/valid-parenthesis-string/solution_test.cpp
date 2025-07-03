#include <gtest/gtest.h>
#include <string>

using namespace std;

struct Solution {
  bool dfs(auto iter, const auto end, int openLeft) {
    if (iter == end) {
      return openLeft == 0;
    }
    auto symbol = *iter;
    if (symbol == ' ') {
      return dfs(++iter, end, openLeft);
    }
    if (symbol == '(') {
      openLeft += 1;
      return dfs(++iter, end, openLeft);
    }
    if (symbol == ')') {
      openLeft -= 1;
      if (openLeft < 0) {
        return false;
      }
      return dfs(++iter, end, openLeft);
    }
    constexpr std::array<char, 3> possibilities = {'(', ' ', ')'};
    for (auto possible : possibilities) {
      *iter = possible;
      if (dfs(iter, end, openLeft)) {
        return true;
      }
    }
    return false;
  };
  bool checkValidString(string &str) { return dfs(str.begin(), str.end(), 0); }
};

// Struct to hold test parameters
struct TestCase {
  string input;
  bool expected;
};

// Parametrized test fixture
class ValidParenthesisStringTest : public ::testing::TestWithParam<TestCase> {};

TEST_P(ValidParenthesisStringTest, HandlesVariousInputs) {
  Solution sol;
  TestCase testCase = GetParam();
  std::cout << testCase.input << std::endl;
  EXPECT_EQ(sol.checkValidString(testCase.input), testCase.expected);
}

// Test cases from problem description and additional edge cases
INSTANTIATE_TEST_SUITE_P(
    ValidParenthesisTests, ValidParenthesisStringTest,
    ::testing::Values(TestCase{"()", true}, TestCase{"(*)", true},
                      TestCase{"(*))", true},
                      TestCase{"", true}, // Empty string is valid
                      TestCase{"(", false}, TestCase{")", false},
                      TestCase{"(*)(", false}, TestCase{"((*)", true},
                      TestCase{"(*))(", false}, TestCase{"*(", false},
                      TestCase{"***", true}, TestCase{"(()*())", true},
                      TestCase{"(()))", false}));
