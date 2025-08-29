#include <cctype>
#include <gtest/gtest.h>
#include <string>

// Assume reverse_letter function is declared above or included from your
// implementation
std::string reverse_letter(const std::string &str) {
  std::string res;
  std::copy_if(str.rbegin(), str.rend(), std::back_inserter(res),
               [](auto letter) { return std::isalpha(letter); });
  return res;
}

struct ReverseLetterTestParam {
  std::string input;
  std::string expected;
};

class ReverseLetterTest
    : public ::testing::TestWithParam<ReverseLetterTestParam> {};

TEST_P(ReverseLetterTest, HandlesVariousStrings) {
  const auto &param = GetParam();
  EXPECT_EQ(reverse_letter(param.input), param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    ReverseLetterTests, ReverseLetterTest,
    ::testing::Values(ReverseLetterTestParam{"krishan", "nahsirk"},
                      ReverseLetterTestParam{"ultr53o?n", "nortlu"},
                      ReverseLetterTestParam{"ab23c", "cba"},
                      ReverseLetterTestParam{"krish21an", "nahsirk"}));
