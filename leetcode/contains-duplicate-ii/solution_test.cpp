#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unordered_map>
#include <vector>

struct Solution {
  bool containsNearbyDuplicate(const std::vector<int> &nums,
                               const std::size_t maxDiff) {
    if (nums.size() == 1 or maxDiff == 0UL) {
      return false;
    }
    std::unordered_map<int, std::size_t> seen;
    for (auto idx = 0UL, size = nums.size(); idx < size; ++idx) {
      auto cur = nums[idx];
      auto iter = seen.find(cur);
      if (iter != seen.cend()) {
        auto another = iter->second;
        auto diff = idx - another;
        if (diff <= maxDiff) {
          return true;
        }
        iter->second = idx;
      } else {
        seen.emplace(cur, idx);
      }
    }
    return false;
  }
};

struct ContainsDuplicateIITest : ::testing::Test {};

TEST_F(ContainsDuplicateIITest, LeetCode) {
  {
    SCOPED_TRACE("1");
    std::vector nums{1, 2, 3, 1};
    int maxDiff = 3;
    auto res = Solution().containsNearbyDuplicate(nums, maxDiff);
    EXPECT_TRUE(res);
  }
  {
    SCOPED_TRACE("2");
    std::vector nums{1, 0, 1, 1};
    int maxDiff = 1;
    auto res = Solution().containsNearbyDuplicate(nums, maxDiff);
    EXPECT_TRUE(res);
  }
  {
    SCOPED_TRACE("3");
    std::vector nums{1, 2, 3, 1, 2, 3};
    int maxDiff = 2;
    auto res = Solution().containsNearbyDuplicate(nums, maxDiff);
    EXPECT_FALSE(res);
  }
}