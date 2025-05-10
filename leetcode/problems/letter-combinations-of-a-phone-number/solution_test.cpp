#include "gmock/gmock.h"
#include <gtest/gtest.h>

#include <array>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

#include <ranges>
namespace rng = std::ranges;
namespace view = std::views;

struct Solution {
  static constexpr std::array<std::string_view, 8> lut{
      "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
  vector<string> letterCombinations(std::string_view digits) {
    if (digits.empty()) {
      return {};
    }
    struct {
      auto operator()(int index = 0) {
        if (index == digits.length()) {
          auto &&res =
              std::views::single(stack) | view::join | rng::to<std::string>();
          result.emplace_back(std::move(res));
          return *this;
        }
        auto digit = digits.at(index);
        auto digitCode = digit - int('0');
        auto lutIdx = digitCode - 2;
        auto &&letters = lut.at(lutIdx);
        for (auto i = 0UL; i < letters.length(); ++i) {
          stack.push_back(letters[i]);
          operator()(index + 1);
          stack.pop_back();
        }
        return *this;
      }
      std::string_view digits;
      vector<string> result;
      vector<char> stack;
    } impl{digits};
    return std::move(impl().result);
  }
};

struct LetterCombinationTest
    : public testing::TestWithParam<tuple<string, vector<string>>> {
  Solution solution;
};

TEST_P(LetterCombinationTest, ValidatesCombinationResults) {
  auto [input, expected] = GetParam();
  auto actual = solution.letterCombinations(input);
  EXPECT_THAT(actual, ::testing::UnorderedElementsAreArray(expected));
}

INSTANTIATE_TEST_SUITE_P(
    LetterCombinationTests, LetterCombinationTest,
    testing::Values(
        make_tuple("", vector<string>{}),               // Empty input
        make_tuple("2", vector<string>{"a", "b", "c"}), // Single digit
        make_tuple("23", vector<string>{"ad", "ae", "af", "bd", "be", "bf",
                                        "cd", "ce", "cf"}), // Two digits
        make_tuple("9",
                   vector<string>{"w", "x", "y", "z"}), // Digit with 4 letters
        make_tuple("234",
                   vector<string>{"adg", "adh", "adi", "aeg", "aeh", "aei",
                                  "afg", "afh", "afi", "bdg", "bdh", "bdi",
                                  "beg", "beh", "bei", "bfg", "bfh", "bfi",
                                  "cdg", "cdh", "cdi", "ceg", "ceh", "cei",
                                  "cfg", "cfh", "cfi"}), // Three digits
        make_tuple("79",
                   vector<string>{"pw", "px", "py", "pz", "qw", "qx", "qy",
                                  "qz", "rw", "rx", "ry", "rz", "sw", "sx",
                                  "sy", "sz"}), // Digits with 4 and 4 letters
        make_tuple("22", vector<string>{"aa", "ab", "ac", "ba", "bb", "bc",
                                        "ca", "cb", "cc"}) // Repeated digit
        ),
    [](const testing::TestParamInfo<LetterCombinationTest::ParamType> &info) {
      string name = "Input_" + get<0>(info.param);
      // Replace non-alphanumeric characters with underscore
      return name;
    });

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}