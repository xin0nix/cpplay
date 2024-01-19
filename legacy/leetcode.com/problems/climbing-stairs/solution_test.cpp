#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<BasicSolution, BottomUpSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  EXPECT_EQ(this->getSolution().climbStairs(0), 0);
}

TYPED_TEST(SolutionTest, Test2) {
  EXPECT_EQ(this->getSolution().climbStairs(1), 1);
  EXPECT_EQ(this->getSolution().climbStairs(2), 2);
  EXPECT_EQ(this->getSolution().climbStairs(3), 3);
  EXPECT_EQ(this->getSolution().climbStairs(5), 8);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
