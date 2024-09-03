#include <algorithm>
#include <ranges>
#include <vector>

#include <gtest/gtest.h>

using std::vector;

struct Solution {
  int minimumDifference(vector<int> &nums, size_t k) {
    std::ranges::sort(nums);
    int answer = nums.back() - nums.front();
    auto l = 0U;
    auto r = k - 1;
    auto size = nums.size();
    for (; r < size; ++l, ++r) {
      answer = std::min(answer, nums[r] - nums[l]);
    }
    return answer;
  }
};

TEST(MinDiffTest, Leet1) {
  vector nums{90};
  EXPECT_EQ(Solution().minimumDifference(nums, 1), 0);
}

TEST(MinDiffTest, Leet2) {
  vector nums{9, 4, 1, 7};
  EXPECT_EQ(Solution().minimumDifference(nums, 2), 2);
}

TEST(MinDiffTest, Custom1) {
  vector nums{2, 8, 9, 6, 2, 0, 5};
  EXPECT_EQ(Solution().minimumDifference(nums, 4), 4);
}

TEST(MinDiffTest, Custom2) {
  vector nums{2, 8, 9, 6, 2, 0, 5};
  EXPECT_EQ(Solution().minimumDifference(nums, 2), 0);
}

TEST(MinDiffTest, Custom3) {
  vector nums{2, 8, 9, 6, 2, 0, 5};
  EXPECT_EQ(Solution().minimumDifference(nums, 3), 2);
}
