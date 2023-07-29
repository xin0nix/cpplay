#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<BasicSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<int> numbers;
  EXPECT_EQ(this->getSolution().solve(numbers), false);
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{1, 2, 3, 1};
  EXPECT_EQ(this->getSolution().solve(numbers), true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
