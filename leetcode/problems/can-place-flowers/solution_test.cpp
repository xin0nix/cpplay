#include <gtest/gtest.h>

#include <vector>

using std::vector;

struct BruteForce {
  bool canPlaceFlowers(const vector<int> &flowerbed, const int n) {
    if (n == 0) {
      return true;
    }
    int counter = 0;
    int flowers = 0;
    const int sz = static_cast<int>(flowerbed.size());
    for (int i = -1; i <= sz; i += 1) {
      if (i == -1 || i == sz) {
        counter += 1;
      } else {
        if (flowerbed[i] == 0) {
          counter += 1;
        } else {
          counter = 0;
        }
      }
      if (counter == 3) {
        flowers += 1;
        counter = 1;
      }
      if (flowers == n) {
        return true;
      }
    }
    return false;
  }
};

TEST(CanPlaceFlowersTest, Example1) {
  BruteForce solution;
  vector flowerbed = {1, 0, 0, 0, 1};
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 1));
  EXPECT_FALSE(solution.canPlaceFlowers(flowerbed, 2));
}

TEST(CanPlaceFlowersTest, SingleVacantElement) {
  BruteForce solution;
  vector flowerbed = {0};
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 0));
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 1));
}

TEST(CanPlaceFlowersTest, SingleElementOccupied) {
  BruteForce solution;
  vector flowerbed = {1};
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 0));
  EXPECT_FALSE(solution.canPlaceFlowers(flowerbed, 1));
  EXPECT_FALSE(solution.canPlaceFlowers(flowerbed, 2));
}

TEST(CanPlaceFlowersTest, NoSpace) {
  BruteForce solution;
  vector flowerbed = {1, 1, 1, 1, 1};
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 0));
  EXPECT_FALSE(solution.canPlaceFlowers(flowerbed, 1));
}

TEST(CanPlaceFlowersTest, IncrementFrom0To4) {
  BruteForce solution;
  vector flowerbed = {0, 0, 0, 0, 0};
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 0));
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 1));
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 2));
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 3));
  EXPECT_FALSE(solution.canPlaceFlowers(flowerbed, 4));
}

TEST(CanPlaceFlowersTest, LeftCornerCase) {
  BruteForce solution;
  vector flowerbed = {0, 0, 1, 1, 1, 1};
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 1));
  EXPECT_FALSE(solution.canPlaceFlowers(flowerbed, 2));
}

TEST(CanPlaceFlowersTest, MiddleCase) {
  BruteForce solution;
  vector flowerbed = {1, 1, 0, 0, 0, 1, 1};
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 1));
  EXPECT_FALSE(solution.canPlaceFlowers(flowerbed, 2));
}

TEST(CanPlaceFlowersTest, RightCornerCase) {
  BruteForce solution;
  vector flowerbed = {1, 1, 1, 1, 0, 0};
  EXPECT_TRUE(solution.canPlaceFlowers(flowerbed, 1));
  EXPECT_FALSE(solution.canPlaceFlowers(flowerbed, 2));
}