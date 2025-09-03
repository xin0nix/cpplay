#include <gtest/gtest.h>
#include <string>

// Declaration of the function to be tested
bool XO(const std::string &str) {
  int counter{0};
  for (char letter : str) {
    if (letter == 'x' or letter == 'X') {
      counter++;
    }
    if (letter == 'o' or letter == 'O') {
      counter--;
    }
  }
  return counter == 0;
}

struct XOTestParam {
  std::string input;
  bool expected;
};

class XOTest : public ::testing::TestWithParam<XOTestParam> {};

TEST_P(XOTest, ReturnsCorrectBoolean) {
  const auto &param = GetParam();
  EXPECT_EQ(XO(param.input), param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    XOTests, XOTest,
    ::testing::Values(XOTestParam{"ooxx", true}, XOTestParam{"xooxx", false},
                      XOTestParam{"ooxXm", true},
                      XOTestParam{"zpzpzpp", true}, // no 'x' or 'o' means true
                      XOTestParam{"zzoo", false},
                      XOTestParam{"", true},       // empty string edge case
                      XOTestParam{"XxOoXo", true}, // additional mixed case
                      XOTestParam{"xxooXXOO", true}
                      // balanced uppercase and lowercase
                      ));
