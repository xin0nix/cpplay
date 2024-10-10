#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

using ::testing::ElementsAre;

struct Solution {
  // Function to be tested
  std::vector<int> nextGreaterElement(std::vector<int> &nums1,
                                      std::vector<int> &nums2) {
    std::vector<int> result;
    result.reserve(nums1.size());
    // Given nums1[i], find nums2[j] such that nums2[j] == nums1[i]
    for (const auto n : nums1) {
      auto it = std::find(nums2.cbegin(), nums2.cend(), n);
      // Look up for nums2[k], such that k > j, nums2[k] > nums2[j]
      auto next = std::find_if(it, nums2.cend(), [n](auto x) { return x > n; });
      if (next != nums2.cend()) {
        result.push_back(*next);
        continue;
      }
      // Otherwise, use -1
      result.push_back(-1);
    }
    return result;
  }
};

TEST(NextGreaterElementTest, BasicTest) {
  std::vector<int> nums1 = {4, 1, 2};
  std::vector<int> nums2 = {1, 3, 4, 2};
  EXPECT_THAT(Solution().nextGreaterElement(nums1, nums2),
              ElementsAre(-1, 3, -1));
}

TEST(NextGreaterElementTest, EmptyNums1Test) {
  std::vector<int> nums1 = {};
  std::vector<int> nums2 = {1, 2, 3};
  std::vector<int> expected = {};
  std::vector<int> actual = Solution().nextGreaterElement(nums1, nums2);
  EXPECT_EQ(expected, actual);
}

TEST(NextGreaterElementTest, ExampleTest1) {
  std::vector<int> nums1 = {1, 3, 2};
  std::vector<int> nums2 = {4, 2, 3, 0, 1, 6, 5};
  EXPECT_THAT(Solution().nextGreaterElement(nums1, nums2),
              ElementsAre(6, 6, 3));
}

TEST(NextGreaterElementTest, AnotherExampleTest) {
  std::vector<int> nums1 = {1, 3, 2, 6, 5, 0};
  std::vector<int> nums2 = {3, 0, 4, 1, 2, 5, 6};
  EXPECT_THAT(Solution().nextGreaterElement(nums1, nums2),
              ElementsAre(2, 4, 5, -1, 6, 4));
}