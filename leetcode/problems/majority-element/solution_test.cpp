#include <gtest/gtest.h>
#include <limits>
#include <random>
#include <vector>

using std::vector;

// @see: https://leetcode.com/problems/majority-element/
struct BruteForce {
  int majorityElement(vector<int> &nums) {
    std::sort(nums.begin(), nums.end());
    return nums[nums.size() / 2];
  }
};

TEST(MajorityElementTest, Example1Test) {
  std::vector<int> nums = {3, 2, 3};
  int expected = 3;
  int actual = BruteForce().majorityElement(nums);
  EXPECT_EQ(expected, actual);
}

TEST(MajorityElementTest, Example2Test) {
  std::vector<int> nums = {2, 2, 1, 1, 1, 2, 2};
  int expected = 2;
  int actual = BruteForce().majorityElement(nums);
  EXPECT_EQ(expected, actual);
}

TEST(MajorityElementTest, SingleElementTest) {
  std::vector<int> nums = {1};
  int expected = 1;
  int actual = BruteForce().majorityElement(nums);
  EXPECT_EQ(expected, actual);
}

TEST(MajorityElementTest, Mixed1sAnd2sMajorityTest) {
  std::vector<int> nums = {1, 2, 1, 2, 1, 2, 2};
  int expected = 2;
  int actual = BruteForce().majorityElement(nums);
  EXPECT_EQ(expected, actual);
}

TEST(MajorityElementTest, LargeInputTest) {
  std::vector<int> nums(10000, 1);
  for (int i = 0; i < 4000; ++i) {
    nums[i] = 2;
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(nums.begin(), nums.end(), g);
  int expected = 1;
  int actual = BruteForce().majorityElement(nums);
  EXPECT_EQ(expected, actual);
}

TEST(MajorityElementTest, VariedInputTest) {
  std::vector<int> nums = {-1, -1, -1, 0, 1, 1, 1, -1, 0, -1, -1};
  int expected = -1;
  int actual = BruteForce().majorityElement(nums);
  EXPECT_EQ(expected, actual);
}

TEST(MajorityElementTest, ShuffleTest) {
  std::vector<int> nums{-7, -7, -6, -3, 2, 2, 2, 2, 2};
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(nums.begin(), nums.end(), g);
  int expected = 2;
  int actual = BruteForce().majorityElement(nums);
  EXPECT_EQ(expected, actual);
}
