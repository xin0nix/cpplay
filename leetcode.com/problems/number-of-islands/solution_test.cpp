#include "solution.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<SetNDequeSolution, RewriterSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<std::vector<char>> grid;
  EXPECT_THAT(this->getSolution().numIslands(grid), 0);
}

TYPED_TEST(SolutionTest, Test2) {
  // clang-format off
  std::vector<std::vector<char>> grid{
      {'1', '1', '1', '0', '0', '0'},
      {'1', '1', '1', '0', '0', '0'},
      {'1', '1', '1', '0', '0', '0'},
      {'0', '0', '0', '1', '0', '0'},
      {'0', '0', '0', '0', '1', '1'},
      {'0', '0', '0', '0', '1', '1'},
  };
  // clang-format on
  EXPECT_THAT(this->getSolution().numIslands(grid), 3);
}

TYPED_TEST(SolutionTest, Test3) {
  // clang-format off
  std::vector<std::vector<char>> grid{
      {'1', '1', '1'},
      {'0', '1', '0'},
      {'1', '1', '1'},
  };
  // clang-format on
  EXPECT_THAT(this->getSolution().numIslands(grid), 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
