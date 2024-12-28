#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <range/v3/all.hpp>
#include <vector>

struct BruteForce {
  int trap(const std::vector<int> &height) {
    const int len = (int)height.size();
    int sum = 0;
    for (int k = 0; k < len; ++k) {
      int lMax = 0;
      for (int left = k - 1; left >= 0; --left) {
        lMax = std::max(lMax, height[left]);
      }
      int rMax = 0;
      for (int right = k + 1; right < len; ++right) {
        rMax = std::max(rMax, height[right]);
      }
      int potential = std::min(lMax, rMax) - height[k];
      sum += std::max(0, potential);
    }
    return sum;
  }
};

struct TrappingRainWaterTest : ::testing::Test {};

TEST_F(TrappingRainWaterTest, Leet1) {
  std::vector height{0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
  auto res = BruteForce().trap(height);
  EXPECT_EQ(res, 6);
  std::reverse(height.begin(), height.end());
  res = BruteForce().trap(height);
  EXPECT_EQ(res, 6);
}

TEST_F(TrappingRainWaterTest, Leet2) {
  std::vector height{4, 2, 0, 3, 2, 5};
  auto res = BruteForce().trap(height);
  EXPECT_EQ(res, 9);
  std::reverse(height.begin(), height.end());
  res = BruteForce().trap(height);
  EXPECT_EQ(res, 9);
}

TEST_F(TrappingRainWaterTest, Ladder) {
  std::vector height{0, 1, 2, 3};
  auto res = BruteForce().trap(height);
  EXPECT_EQ(res, 0);
  std::reverse(height.begin(), height.end());
  res = BruteForce().trap(height);
  EXPECT_EQ(res, 0);
}

TEST_F(TrappingRainWaterTest, Empty) {
  std::vector height{0, 0, 0, 0};
  auto res = BruteForce().trap(height);
  EXPECT_EQ(res, 0);
}

TEST_F(TrappingRainWaterTest, Full) {
  std::vector height{100, 100, 100, 100};
  auto res = BruteForce().trap(height);
  EXPECT_EQ(res, 0);
}

TEST_F(TrappingRainWaterTest, MiddlePole) {
  std::vector height{0, 0, 42, 0, 0};
  auto res = BruteForce().trap(height);
  EXPECT_EQ(res, 0);
}

TEST_F(TrappingRainWaterTest, ComplexA) {
  std::vector height{0, 2, 0, 4, 1, 0, 3, 6, 2, 3, 4, 3};
  auto res = BruteForce().trap(height);
  EXPECT_EQ(res, 13);
  std::reverse(height.begin(), height.end());
  res = BruteForce().trap(height);
  EXPECT_EQ(res, 13);
}

TEST_F(TrappingRainWaterTest, ComplexB) {
  std::vector height{5, 3, 2, 0, 1, 6, 0};
  auto res = BruteForce().trap(height);
  EXPECT_EQ(res, 14);
  std::reverse(height.begin(), height.end());
  res = BruteForce().trap(height);
  EXPECT_EQ(res, 14);
}