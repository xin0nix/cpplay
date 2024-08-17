#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ranges>
#include <string>

using std::string;
using std::vector;

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

TEST(LongesPrefix, Leet) {
  EXPECT_EQ(
      RangeBasedSolution().longestCommonPrefix({"flower", "flow", "flight"}),
      "fl");
  EXPECT_EQ(RangeBasedSolution().longestCommonPrefix({"dog", "racecar", "car"}),
            "");
}
