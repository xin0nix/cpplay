#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<BruteForceSolution, DivideConquerSolution, SlidingWindowSolution>
    Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<int> numbers;
  EXPECT_EQ(this->getSolution().maxSubArray(numbers), 0);
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{1, 2, 3, -1};
  EXPECT_EQ(this->getSolution().maxSubArray(numbers), 6);
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> numbers{1, 2, -1, -1, -1};
  EXPECT_EQ(this->getSolution().maxSubArray(numbers), 3);
}

TYPED_TEST(SolutionTest, Test4) {
  std::vector<int> numbers{0, -1, -1, 2, 3};
  EXPECT_EQ(this->getSolution().maxSubArray(numbers), 5);
}

TYPED_TEST(SolutionTest, Test5) {
  std::vector<int> numbers{1};
  EXPECT_EQ(this->getSolution().maxSubArray(numbers), 1);
}

TYPED_TEST(SolutionTest, Test6) {
  std::vector<int> numbers{1, 2, 3};
  EXPECT_EQ(this->getSolution().maxSubArray(numbers), 6);
}

TYPED_TEST(SolutionTest, Test7) {
  std::vector<int> numbers{-9, -1, -7};
  EXPECT_EQ(this->getSolution().maxSubArray(numbers), -1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
