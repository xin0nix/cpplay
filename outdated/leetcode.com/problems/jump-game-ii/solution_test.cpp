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
  std::vector<int> numbers;
  EXPECT_EQ(this->getSolution().jump(numbers), 0);
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{1, 2, 3, 4};
  EXPECT_EQ(this->getSolution().jump(numbers), 2);
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> numbers{2, 3, 1, 1, 4};
  EXPECT_EQ(this->getSolution().jump(numbers), 2);
}

TYPED_TEST(SolutionTest, Test4) {
  std::vector<int> numbers{2, 2, 0, 1, 1, 0};
  EXPECT_EQ(this->getSolution().jump(numbers), 4);
}

TYPED_TEST(SolutionTest, Test5) {
  std::vector<int> numbers{1, 2, 3, 0, 0, 0};
  EXPECT_EQ(this->getSolution().jump(numbers), 3);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
