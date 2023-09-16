#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<BFSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<int> numbers{1};
  EXPECT_EQ(this->getSolution().rob(numbers), 1);
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{1, 2, 3, 1};
  EXPECT_EQ(this->getSolution().rob(numbers), 4);
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> numbers{2, 7, 9, 3, 1};
  EXPECT_EQ(this->getSolution().rob(numbers), 12);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
