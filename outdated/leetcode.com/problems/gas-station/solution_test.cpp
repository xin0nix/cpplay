#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<GreedySolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<int> gas;
  std::vector<int> cost;
  EXPECT_EQ(this->getSolution().canCompleteCircuit(gas, cost), -1);
}

TYPED_TEST(SolutionTest, Test1_2) {
  std::vector<int> gas{1, 2, 1};
  std::vector<int> cost{2, 1, 3};
  EXPECT_EQ(this->getSolution().canCompleteCircuit(gas, cost), -1);
}

TYPED_TEST(SolutionTest, Test1_3) {
  std::vector<int> gas{2, 3, 4};
  std::vector<int> cost{3, 4, 3};
  EXPECT_EQ(this->getSolution().canCompleteCircuit(gas, cost), -1);
}

TYPED_TEST(SolutionTest, Test1_4) {
  std::vector<int> gas{3, 1, 1};
  std::vector<int> cost{1, 2, 2};
  EXPECT_EQ(this->getSolution().canCompleteCircuit(gas, cost), 0);
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> gas{1, 2, 3, 1};
  std::vector<int> cost{3, 2, 1, 1};
  // -2, 0, 2, 0
  EXPECT_EQ(this->getSolution().canCompleteCircuit(gas, cost), 2);
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> gas{1, 2, 3, 4, 5};
  std::vector<int> cost{3, 4, 5, 1, 2};
  EXPECT_EQ(this->getSolution().canCompleteCircuit(gas, cost), 3);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
