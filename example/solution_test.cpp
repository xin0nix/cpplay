#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<SetSolution, SortSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<int> numbers;
  EXPECT_EQ(this->getSolution().containsDuplicate(numbers), false);
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{1, 2, 3, 1};
  EXPECT_EQ(this->getSolution().containsDuplicate(numbers), true);
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> numbers{1, 2, 3, 4};
  EXPECT_EQ(this->getSolution().containsDuplicate(numbers), false);
}

TYPED_TEST(SolutionTest, Test4) {
  std::vector<int> numbers{INT32_MIN, INT32_MAX};
  EXPECT_EQ(this->getSolution().containsDuplicate(numbers), false);
}

TYPED_TEST(SolutionTest, Test5) {
  std::vector<int> numbers{INT32_MIN, INT32_MAX, INT32_MIN};
  EXPECT_EQ(this->getSolution().containsDuplicate(numbers), true);
}

TYPED_TEST(SolutionTest, Test6) {
  std::vector<int> numbers{-1, 0, 1, -1, 0, 1, 2, 3, 4};
  EXPECT_EQ(this->getSolution().containsDuplicate(numbers), true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
