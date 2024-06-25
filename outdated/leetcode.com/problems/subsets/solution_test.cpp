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
  EXPECT_THAT(this->getSolution().subsets(numbers),
              testing::UnorderedElementsAre());
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<int> numbers{1, 2, 3};
  auto r = this->getSolution().subsets(numbers);
  EXPECT_EQ(r.size(), 8);
  EXPECT_THAT(r, testing::Contains(testing::UnorderedElementsAre()));
  EXPECT_THAT(r, testing::Contains(testing::UnorderedElementsAre(1)));
  EXPECT_THAT(r, testing::Contains(testing::UnorderedElementsAre(2)));
  EXPECT_THAT(r, testing::Contains(testing::UnorderedElementsAre(3)));
  EXPECT_THAT(r, testing::Contains(testing::UnorderedElementsAre(1, 2)));
  EXPECT_THAT(r, testing::Contains(testing::UnorderedElementsAre(2, 3)));
  EXPECT_THAT(r, testing::Contains(testing::UnorderedElementsAre(1, 3)));
  EXPECT_THAT(r, testing::Contains(testing::UnorderedElementsAre(1, 2, 3)));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
