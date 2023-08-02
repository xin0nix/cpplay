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
  EXPECT_THAT(this->getSolution().topKFrequent(numbers, 0),
              testing::ElementsAre());
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{1, 2, 3, 1};
  EXPECT_THAT(this->getSolution().topKFrequent(numbers, 1),
              testing::UnorderedElementsAre(1));
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<int> numbers{-1, 2, 3, -1};
  EXPECT_THAT(this->getSolution().topKFrequent(numbers, 1),
              testing::UnorderedElementsAre(-1));
}

TYPED_TEST(SolutionTest, Test4) {
  std::vector<int> numbers{1, 2, 3, 5, -1, -2, -3, -4, -5, 2, 2, -2, -2, 3, 3};
  EXPECT_THAT(this->getSolution().topKFrequent(numbers, 3),
              testing::UnorderedElementsAre(2, -2, 3));
}

TYPED_TEST(SolutionTest, Test5) {
  std::vector<int> numbers{1, 1, 1, 2, 2, 3};
  EXPECT_THAT(this->getSolution().topKFrequent(numbers, 2),
              testing::UnorderedElementsAre(1, 2));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
