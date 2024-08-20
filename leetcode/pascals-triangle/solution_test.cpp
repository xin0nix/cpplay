#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace detail {
std::vector<std::string> stringify(const std::vector<std::vector<int>> &input) {
  std::vector<std::string> result;
  result.reserve(input.size());
  for (const auto &inner : input) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < inner.size(); ++i) {
      ss << std::to_string(inner[i]);
      if (i != inner.size() - 1) {
        ss << ",";
      }
    }
    ss << "]";
    result.push_back(ss.str());
  }
  return result;
}
} // namespace detail

using std::vector;
using ::testing::ElementsAre;

struct Solution {
  vector<vector<int>> generate(int size) {
    if (size == 0)
      return {};
    vector<vector<int>> result = {{1}};
    result.reserve(static_cast<size_t>(size));
    if (size == 1)
      return result;
    result.push_back(vector{1, 1});
    if (size == 2)
      return result;
    for (int row = 2; row < size; ++row) {
      vector<int> nums(row + 1, 1);
      const auto &numsAbove = result[row - 1];
      for (int col = 1; col < row; ++col) {
        nums[col] = numsAbove[col - 1] + numsAbove[col];
      }
      result.push_back(std::move(nums));
    }
    return result;
  }
};

TEST(PascalTriangle, Leet) {
  {
    SCOPED_TRACE("Leet 1");
    auto res = detail::stringify(Solution().generate(5));
    EXPECT_THAT(res, ElementsAre("[1]", "[1,1]", "[1,2,1]", "[1,3,3,1]",
                                 "[1,4,6,4,1]"));
  }
}
