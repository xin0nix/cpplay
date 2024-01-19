#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<BFSolution, BottomUpSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<int> cost{1, 100};
  EXPECT_EQ(this->getSolution().minCostClimbingStairs(cost), 1);
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> cost{10, 15, 20};
  EXPECT_EQ(this->getSolution().minCostClimbingStairs(cost), 15);
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> cost{1, 100, 1, 1, 1, 100, 1, 1, 100, 1};
  EXPECT_EQ(this->getSolution().minCostClimbingStairs(cost), 6);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
