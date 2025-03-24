#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

#include <ranges>
namespace rng = std::ranges;
namespace view = std::views;

struct Solution {
  int singleNumber(std::vector<int> &nums) {
    return rng::fold_left(nums, 0, [](int lhs, int rhs) { return lhs ^ rhs; });
  }
};

struct SolutionTest : ::testing::Test {};

TEST(SolutionTest, SingleElement) {
  Solution s;
  std::vector<int> nums{5};
  EXPECT_EQ(s.singleNumber(nums), 5);
}

TEST(SolutionTest, MultipleElementsWithSingle) {
  Solution s;
  std::vector<int> nums{2, 2, 1};
  EXPECT_EQ(s.singleNumber(nums), 1);
}

TEST(SolutionTest, LargerVectorWithSingle) {
  Solution s;
  std::vector<int> nums{4, 1, 2, 1, 2};
  EXPECT_EQ(s.singleNumber(nums), 4);
}

TEST(SolutionTest, NegativeNumbers) {
  Solution s;
  std::vector<int> nums{-1, 2, -1, 2, 4};
  EXPECT_EQ(s.singleNumber(nums), 4);
}