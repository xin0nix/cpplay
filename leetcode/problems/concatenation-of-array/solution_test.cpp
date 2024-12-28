#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

struct BruteForce {
  std::vector<int> getConcatenation(std::vector<int> &nums) {
    auto res = nums;
    res.reserve(nums.size());
    std::copy(nums.begin(), nums.end(), std::back_inserter(res));
    return res;
  }
};

TEST(ConcatArrayTest, Leet1) {
  std::vector nums{1, 2, 1};
  auto res = BruteForce().getConcatenation(nums);
  ASSERT_THAT(res, ::testing::ElementsAre(1, 2, 1, 1, 2, 1));
}
