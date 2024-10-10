#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

using ::testing::ElementsAre;

// Function to be tested
std::vector<int> nextGreaterElement(std::vector<int> &nums1,
                                    std::vector<int> &nums2) {
  return {};
}

TEST(NextGreaterElementTest, BasicTest) {
  std::vector<int> nums1 = {4, 1, 2};
  std::vector<int> nums2 = {1, 3, 4, 2};
  EXPECT_THAT(nextGreaterElement(nums1, nums2), ElementsAre(-1, 3, -1));
}

TEST(NextGreaterElementTest, NoGreaterElementTest) {
  std::vector<int> nums1 = {2, 4};
  std::vector<int> nums2 = {1, 2, 3};
  EXPECT_THAT(nextGreaterElement(nums1, nums2), ElementsAre(-1, -1));
}

TEST(NextGreaterElementTest, EmptyNums1Test) {
  std::vector<int> nums1 = {};
  std::vector<int> nums2 = {1, 2, 3};
  std::vector<int> expected = {};
  std::vector<int> actual = nextGreaterElement(nums1, nums2);
  EXPECT_EQ(expected, actual);
}

TEST(NextGreaterElementTest, ExampleTest1) {
  std::vector<int> nums1 = {1, 3, 2};
  std::vector<int> nums2 = {4, 2, 3, 0, 1, 6, 5};
  EXPECT_THAT(nextGreaterElement(nums1, nums2), ElementsAre(6, 6, 6));
}

TEST(NextGreaterElementTest, AnotherExampleTest) {
  std::vector<int> nums1 = {1, 3, 2, 6, 5, 0};
  std::vector<int> nums2 = {3, 0, 4, 1, 2, 5, 6};
  EXPECT_THAT(nextGreaterElement(nums1, nums2), ElementsAre(2, 4, 5, -1, 6, 4));
}