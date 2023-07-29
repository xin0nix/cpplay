#include "solution.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<MapSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<int> numbers{2, 7, 11, 15};
  int target = 9;
  EXPECT_THAT(this->getSolution().twoSum(numbers, target),
              testing::UnorderedElementsAre(0, 1));
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{3, 2, 4};
  int target = 6;
  EXPECT_THAT(this->getSolution().twoSum(numbers, target),
              testing::UnorderedElementsAre(1, 2));
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> numbers{3, 3};
  int target = 6;
  EXPECT_THAT(this->getSolution().twoSum(numbers, target),
              testing::UnorderedElementsAre(0, 1));
}

TYPED_TEST(SolutionTest, Test4) {
  std::vector<int> numbers{1, 2, 3, -10, 0, 10};
  int target = 0;
  EXPECT_THAT(this->getSolution().twoSum(numbers, target),
              testing::UnorderedElementsAre(3, 5));
}

TYPED_TEST(SolutionTest, Test5) {
  std::vector<int> numbers{-100, 2, 3, 4, 7};
  int target = -93;
  EXPECT_THAT(this->getSolution().twoSum(numbers, target),
              testing::UnorderedElementsAre(0, 4));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
