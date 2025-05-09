#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <deque>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include <ranges>
namespace rng = std::ranges;
namespace view = std::views;

struct Solution {
  static constexpr bool isPalindrome(std::string_view str) {
    for (int left = 0UL, right = (int)str.size() - 1; left <= right;
         left++, right--) {
      if (str[left] != str[right]) {
        return false;
      }
    }
    return !str.empty();
  }

  vector<vector<string>> partition(std::string_view str) {
    struct Impl {
      void operator()(int start = 0) {
        const auto len = (int)wholeString.length();
        // We reached the end, this means that the stack contains a proper path
        // (aka partition), so memorize it and rewind back to the previous node
        if (start == len) {
          auto &&part =
              stack |
              view::transform([](auto strv) { return std::string(strv); }) |
              rng::to<std::vector<std::string>>();
          partitions.push_back(std::move(part));
          return;
        }
        // Now we should explore all possible substrings with the given start
        for (int end = start + 1; end <= len; ++end) {
          std::string_view subStr = wholeString.substr(start, end - start);
          if (isPalindrome(subStr)) {
            stack.push_back(subStr);
            operator()(end);
            stack.pop_back();
          }
        }
      }
      std::string_view wholeString;
      vector<vector<string>> partitions;
      deque<string_view> stack;
    } impl{.wholeString = str};
    impl();
    return std::move(impl.partitions);
  }
};

namespace {
std::ostream &operator<<(std::ostream &oStream, const vector<string> &data) {
  oStream << "|";
  for (const auto &subs : data) {
    oStream << subs << "|";
  }
  return oStream;
}
} // namespace

struct PalindromePartitionTest
    : public testing::TestWithParam<
          std::tuple<std::string, std::vector<std::vector<std::string>>>> {
  Solution solution;
};

TEST_P(PalindromePartitionTest, ValidPartitions) {
  auto [input, expected] = GetParam();
  auto result = solution.partition(input);

  auto stringify = [](auto &subStrs) {
    std::stringstream sStream;
    sStream << subStrs;
    return sStream.str();
  };
  auto reResult =
      result | view::transform(stringify) | rng::to<vector<string>>();
  auto reExpected =
      expected | view::transform(stringify) | rng::to<vector<string>>();
  EXPECT_THAT(reResult, ::testing::UnorderedElementsAreArray(reExpected));
}

INSTANTIATE_TEST_SUITE_P(
    PartitionCases, PalindromePartitionTest,
    testing::Values(
        // Основные случаи
        std::make_tuple("abac",
                        std::vector<std::vector<std::string>>{
                            {"a", "b", "a", "c"}, {"aba", "c"}}),
        std::make_tuple("aab",
                        std::vector<std::vector<std::string>>{{"a", "a", "b"},
                                                              {"aa", "b"}}),

        // Краевые случаи
        std::make_tuple("a", std::vector<std::vector<std::string>>{{"a"}}),

        // Все палиндромы
        std::make_tuple("aaa",
                        std::vector<std::vector<std::string>>{{"a", "a", "a"},
                                                              {"a", "aa"},
                                                              {"aa", "a"},
                                                              {"aaa"}}),

        // Нет составных палиндромов
        std::make_tuple("abc", std::vector<std::vector<std::string>>{
                                   {"a", "b", "c"}})));

TEST(IsPalindrome, Ok) {
  EXPECT_FALSE(Solution::isPalindrome(""))
      << "Empty string is not a palindrome";
  EXPECT_TRUE(Solution::isPalindrome("a"))
      << "Single character should be palindrome";
  EXPECT_TRUE(Solution::isPalindrome("aa"))
      << "Two same characters should be palindrome";
  EXPECT_TRUE(!Solution::isPalindrome("ab"))
      << "Different characters should not be palindrome";
  EXPECT_TRUE(Solution::isPalindrome("aba")) << "Odd-length palindrome";
  EXPECT_TRUE(Solution::isPalindrome("abba")) << "Even-length palindrome";
  EXPECT_TRUE(!Solution::isPalindrome("abc")) << "Not a palindrome";
  EXPECT_TRUE(!Solution::isPalindrome("a man a plan a canal panama"))
      << "Spaces included, not palindrome as-is";
  EXPECT_TRUE(Solution::isPalindrome("12321")) << "Numeric palindrome";
  EXPECT_TRUE(!Solution::isPalindrome("1231")) << "Numeric non-palindrome";
}