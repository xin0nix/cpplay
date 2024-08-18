#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ranges>
#include <string>

using std::string;
using std::vector;

struct BasicSolution {
  string longestCommonPrefix(const vector<string> &strs) {
    // Early exit conditions
    if (strs.empty())
      return "";
    if (strs.size() == 1)
      return strs.front();
    // Figure out shortest string
    size_t shortestLen = strs[0].size();
    for (auto &s : strs)
      shortestLen = std::min(s.size(), shortestLen);
    // Iterate over "char columns" and stop on the first mismatch
    std::stringstream ss;
    for (size_t charPos = 0U; charPos < shortestLen; ++charPos) {
      const char cur = strs[0][charPos];
      bool mismatch = false;
      for (const auto &s : strs) {
        if (s[charPos] != cur) {
          mismatch = true;
          break;
        }
      }
      if (mismatch)
        break;
      ss << cur;
    }
    return ss.str();
  }
};

struct RangeBasedSolution {
  string longestCommonPrefix(const vector<string> &strs) {
    namespace ranges = std::ranges;
    if (strs.empty())
      return "";
    if (strs.size() == 1)
      return strs.front();
    size_t shortestLen = ranges::min(
        strs | std::views::transform([](const string &s) { return s.size(); }));
    std::stringstream ss;
    for (auto charPos : ranges::iota_view{0U, shortestLen}) {
      const char cur = strs[0][charPos];
      auto charsColumn =
          strs | ranges::views::transform(
                     [charPos](const string &s) -> char { return s[charPos]; });
      if (ranges::any_of(charsColumn, [cur](char c) { return c != cur; }))
        break;
      ss << cur;
    }
    return ss.str();
  }
};

template <typename S>
concept SolutionConcept = requires(S s, vector<string> v) {
  { s.longestCommonPrefix(v) } -> std::convertible_to<string>;
};

template <SolutionConcept S> struct SolutionTest : ::testing::Test {
  S sol;
  S &getSol() { return sol; }
};

using SolutionTypes = ::testing::Types<RangeBasedSolution, BasicSolution>;

TYPED_TEST_SUITE(SolutionTest, SolutionTypes);

TYPED_TEST(SolutionTest, Leet) {
  auto &Sol = this->getSol();
  EXPECT_EQ(Sol.longestCommonPrefix({"flower", "flow", "flight"}), "fl");
  EXPECT_EQ(Sol.longestCommonPrefix({"dog", "racecar", "car"}), "");
}
