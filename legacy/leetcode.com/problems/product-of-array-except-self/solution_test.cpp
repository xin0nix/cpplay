#include "solution.hpp"
#include <gmock/gmock.h>
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
  EXPECT_THAT(this->getSolution().productExceptSelf(numbers),
              testing::ElementsAre());
}

TYPED_TEST(SolutionTest, Test1_2) {
  std::vector<int> numbers{2, -3};
  EXPECT_THAT(this->getSolution().productExceptSelf(numbers),
              testing::ElementsAre(-3, 2));
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{2, 3, 4, 5};
  EXPECT_THAT(this->getSolution().productExceptSelf(numbers),
              testing::ElementsAre(60, 40, 30, 24));
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> numbers{-1, 0, 2};
  EXPECT_THAT(this->getSolution().productExceptSelf(numbers),
              testing::ElementsAre(0, -2, 0));
}

TYPED_TEST(SolutionTest, Test4) {
  std::vector<int> numbers{-2, -3, -4, -5};
  EXPECT_THAT(this->getSolution().productExceptSelf(numbers),
              testing::ElementsAre(-60, -40, -30, -24));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
