#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>
using std::vector;

class Solution {
public:
  vector<int> findDisappearedNumbers(vector<int> &nums) {
    for (auto cur : nums) {
      auto idx = abs(cur) - 1;
      auto enc = abs(nums[idx]);
      nums[idx] = -enc;
    }
    vector<int> res;
    const auto size = nums.size();
    // Можно опмтимизировать решение если использовать reserve()
    // Но вычисление размера res это отдельная задача (out of scope)
    for (int idx = 0; idx < (int)size; ++idx) {
      auto val = nums[idx];
      if (val > 0) {
        res.push_back(idx + 1);
      }
    }
    return res;
  }
};

struct SolutionTest : ::testing::Test {};

TEST_F(SolutionTest, Leet1) {
  vector nums{4, 3, 2, 7, 8, 2, 3, 1};
  auto res = Solution().findDisappearedNumbers(nums);
  EXPECT_THAT(res, ::testing::UnorderedElementsAre(5, 6));
}

TEST_F(SolutionTest, Leet2) {
  vector nums{1, 1};
  auto res = Solution().findDisappearedNumbers(nums);
  EXPECT_THAT(res, ::testing::UnorderedElementsAre(2));
}

TEST_F(SolutionTest, NoMissing) {
  vector nums{1, 2, 3, 4};
  auto res = Solution().findDisappearedNumbers(nums);
  EXPECT_THAT(res, ::testing::UnorderedElementsAre());
}