#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<DPSolution, GreedySolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<int> numbers;
  EXPECT_EQ(this->getSolution().canJump(numbers), false);
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{1, 2, 3, 1};
  EXPECT_EQ(this->getSolution().canJump(numbers), true);
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> numbers{2, 3, 1, 1, 4};
  EXPECT_EQ(this->getSolution().canJump(numbers), true);
}

TYPED_TEST(SolutionTest, Test4) {
  std::vector<int> numbers{3, 2, 1, 0, 4};
  EXPECT_EQ(this->getSolution().canJump(numbers), false);
}

TYPED_TEST(SolutionTest, Test5) {
  std::vector<int> numbers{1, 2, 3, 0, 0, 0};
  EXPECT_EQ(this->getSolution().canJump(numbers), true);
}

TYPED_TEST(SolutionTest, Test6) {
  std::vector<int> numbers{2, 0, 6, 9, 8, 4, 5, 0, 8, 9, 1, 2, 9, 6, 8, 8, 0, 6,
                           3, 1, 2, 2, 1, 2, 6, 5, 3, 1, 2, 2, 6, 4, 2, 4, 3, 0,
                           0, 0, 3, 8, 2, 4, 0, 1, 2, 0, 1, 4, 6, 5, 8, 0, 7, 9,
                           3, 4, 6, 6, 5, 8, 9, 3, 4, 3, 7, 0, 4, 9, 0, 9, 8, 4,
                           3, 0, 7, 7, 1, 9, 1, 9, 4, 9, 0, 1, 9, 5, 7, 7, 1, 5,
                           8, 2, 8, 2, 6, 8, 2, 2, 7, 5, 1, 7, 9, 6};
  EXPECT_EQ(this->getSolution().canJump(numbers), false);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
