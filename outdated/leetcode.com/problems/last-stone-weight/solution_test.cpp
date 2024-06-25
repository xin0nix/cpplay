#include "solution.hpp"
#include <gtest/gtest.h>

TEST(SolutionTest, Test1) {
  Solution solution;
  std::vector<int> arr{2, 2};
  EXPECT_EQ(solution.lastStoneWeight(arr), 0);
}

TEST(SolutionTest, Test2) {
  Solution solution;
  // 3, 2, 1
  // 1, 1
  // 0
  std::vector<int> arr{1, 2, 3};
  EXPECT_EQ(solution.lastStoneWeight(arr), 0);
}

TEST(SolutionTest, Test3) {
  Solution solution;
  // 17, 13, 1
  // 4, 1
  // 3
  std::vector<int> arr{1, 13, 17};
  EXPECT_EQ(solution.lastStoneWeight(arr), 3);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
