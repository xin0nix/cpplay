#include "solution.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

template <typename T> struct SolutionTest : public testing::Test {
  T solution;
  SolutionTest() = default;
  std::vector<std::vector<int>> solve(std::vector<std::vector<int>> &numbers,
                                      int k) {
    return solution.kClosest(numbers, k);
  }
};

using testing::Types;
typedef Types<MinHeapSolution, NthElementSolution> Implementations;
TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<std::vector<int>> points{
      {0, 0},     {-3, -4}, {0, 15},  {-3, -1}, {9, 7},
      {-11, -21}, {1, 3},   {39, 93}, {-2, -2}, {11, -13},
  };
  std::vector<std::vector<int>> expected{{0, 0}, {-2, -2}, {-3, -1}, {1, 3}};
  EXPECT_THAT(this->solve(points, 4),
              testing::UnorderedElementsAreArray(expected));
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<std::vector<int>> points{
      {3, 3},
      {5, -1},
      {-2, 4},
  };
  std::vector<std::vector<int>> expected{
      {3, 3},
      {-2, 4},
  };
  EXPECT_THAT(this->solve(points, 2),
              testing::UnorderedElementsAreArray(expected));
}

TYPED_TEST(SolutionTest, Test3) {
  std::vector<std::vector<int>> points{{9997, 9997}, {9996, 9998}};
  std::vector<std::vector<int>> expected{{9997, 9997}};
  EXPECT_THAT(this->solve(points, 1),
              testing::UnorderedElementsAreArray(expected));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
