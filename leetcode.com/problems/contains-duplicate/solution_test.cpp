#include "solution.hpp"
#include <gtest/gtest.h>

TEST(SolutionTest, Test1) {
  std::vector<int> numbers;
  SetSolution solution;
  EXPECT_EQ(solution.containsDuplicate(numbers), false);
}

TEST(SolutionTest, Test2) {
  std::vector<int> numbers{1, 2, 3, 1};
  SetSolution solution;
  EXPECT_EQ(solution.containsDuplicate(numbers), true);
}

TEST(SolutionTest, Test3) {
  std::vector<int> numbers{1, 2, 3, 4};
  SetSolution solution;
  EXPECT_EQ(solution.containsDuplicate(numbers), false);
}

TEST(SolutionTest, Test4) {
  std::vector<int> numbers{INT32_MIN, INT32_MAX};
  SetSolution solution;
  EXPECT_EQ(solution.containsDuplicate(numbers), false);
}

TEST(SolutionTest, Test5) {
  std::vector<int> numbers{INT32_MIN, INT32_MAX, INT32_MIN};
  SetSolution solution;
  EXPECT_EQ(solution.containsDuplicate(numbers), true);
}

TEST(SolutionTest, Test6) {
  std::vector<int> numbers{-1, 0, 1, -1, 0, 1, 2, 3, 4};
  SetSolution solution;
  EXPECT_EQ(solution.containsDuplicate(numbers), true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
