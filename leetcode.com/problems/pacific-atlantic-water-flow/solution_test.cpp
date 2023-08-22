#include "solution.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::Types;
using testing::UnorderedElementsAre;
using testing::UnorderedElementsAreArray;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<BreadthFirstSolution, DepthFirstSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<std::vector<int>> grid{{1}};
  std::vector<std::vector<int>> answer{{0, 0}};
  EXPECT_THAT(this->getSolution().pacificAtlantic(grid),
              UnorderedElementsAreArray(answer));
}

TYPED_TEST(SolutionTest, Test2) {
  // clang-format off
  std::vector<std::vector<int>> grid{
      {1,2,2,3,5},
      {3,2,3,4,4},
      {2,4,5,3,1},
      {6,7,1,4,5},
      {5,1,1,2,4}
  };
  // clang-format on
  std::vector<std::vector<int>> answer{{0, 4}, {1, 3}, {1, 4}, {2, 2},
                                       {3, 0}, {3, 1}, {4, 0}};
  EXPECT_THAT(this->getSolution().pacificAtlantic(grid),
              UnorderedElementsAreArray(answer));
}

TYPED_TEST(SolutionTest, Test3) {
  // clang-format off
  std::vector<std::vector<int>> grid{
      {1 ,  2,  3,  4, 5},
      {16, 17, 18, 19, 6},
      {15, 24, 25, 20, 7},
      {14, 23, 22, 21, 8},
      {13, 12, 11, 10, 9}
  };
  // clang-format on
  std::vector<std::vector<int>> answer{
      {{0, 4}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 0},
       {2, 1}, {2, 2}, {2, 3}, {2, 4}, {3, 0}, {3, 1}, {3, 2},
       {3, 3}, {3, 4}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}}};
  EXPECT_THAT(this->getSolution().pacificAtlantic(grid),
              UnorderedElementsAreArray(answer));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
