#include <gtest/gtest.h>

#include <numeric>
#include <unordered_set>
#include <vector>

class Solution {
public:
  bool canPartition(const std::vector<int> &nums) {
    auto total = std::accumulate(nums.cbegin(), nums.cend(), 0);
    if (total % 2 == 1) {
      return false;
    }
    auto target = total / 2;
    std::unordered_set<int> sums{0}; // basic option - nothing
    for (auto num : nums) {
      std::unordered_set<int> update{};
      for (auto sum : sums) {
        auto candidate = num + sum;
        if (candidate == target) {
          return true;
        }
        if (candidate < target) {
          update.insert(candidate);
        }
      }
      sums.insert(update.begin(), update.end());
    }
    return false;
  }
};

struct TestParams {
  std::vector<int> nums;
  bool expected;
};

class PartitionEqualSubsetSumTest
    : public ::testing::TestWithParam<TestParams> {};

TEST_P(PartitionEqualSubsetSumTest, ValidatesPartitionCases) {
  Solution sol;
  const auto &params = GetParam();
  EXPECT_EQ(sol.canPartition(params.nums), params.expected);
}

INSTANTIATE_TEST_SUITE_P(PartitionTests, PartitionEqualSubsetSumTest,
                         ::testing::Values(
                             // Basic valid partition
                             TestParams{{1, 5, 11, 5}, true},
                             // Basic invalid partition (odd total sum)
                             TestParams{{1, 2, 3, 5}, false},
                             // Single element (cannot partition)
                             TestParams{{1}, false},
                             // Two identical elements (valid partition)
                             TestParams{{1, 1}, true},
                             // Two distinct elements (invalid partition)
                             TestParams{{1, 2}, false},
                             // Valid partition with multiple combinations
                             TestParams{{1, 2, 3, 4, 5, 6, 7}, true},
                             // Invalid partition despite even total sum
                             TestParams{{1, 2, 5}, false},
                             // Valid partition with repeated elements
                             TestParams{{3, 3, 3, 3}, true},
                             // Invalid partition with repeated elements
                             TestParams{{3, 3, 3, 1}, false},
                             // Valid partition with mixed elements
                             TestParams{{2, 2, 1, 1}, true},
                             // Large numbers but valid partition
                             TestParams{{100, 1, 1, 100}, true},
                             // Minimal valid case
                             TestParams{{1, 1}, true},
                             // Minimal invalid case
                             TestParams{{99, 1}, false}));
