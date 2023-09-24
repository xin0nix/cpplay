#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<BasicSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test0) {
  std::vector<int> numbers;
  numbers = {1, 2, 5, 4, 3};
  EXPECT_EQ(this->getSolution().longestConsecutive(numbers), 5);
  numbers = {1, 2, 3, 4, 5};
  EXPECT_EQ(this->getSolution().longestConsecutive(numbers), 5);
  numbers = {5, 4, 3, 2, 1};
  EXPECT_EQ(this->getSolution().longestConsecutive(numbers), 5);
  numbers = {-1, -2, 0, 1, 2};
  EXPECT_EQ(this->getSolution().longestConsecutive(numbers), 5);
  numbers = {-99, -98, -97, -96, -95, 100, 101, 102};
  EXPECT_EQ(this->getSolution().longestConsecutive(numbers), 5);
}

TYPED_TEST(SolutionTest, Test1) {
  std::vector<int> numbers;
  EXPECT_EQ(this->getSolution().longestConsecutive(numbers), 0);
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{100, 4, 200, 1, 3, 2};
  EXPECT_EQ(this->getSolution().longestConsecutive(numbers), 4);
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> numbers{0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
  EXPECT_EQ(this->getSolution().longestConsecutive(numbers), 9);
}

TYPED_TEST(SolutionTest, Test4) {
  std::vector<int> numbers{3, -1, 2, -3, 1, -2, -4};
  EXPECT_EQ(this->getSolution().longestConsecutive(numbers), 4);
}

TYPED_TEST(SolutionTest, Test5) {
  std::vector<int> numbers{1, 3, 2, 1, 2, 3, 1, 2, 3};
  EXPECT_EQ(this->getSolution().longestConsecutive(numbers), 3);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
