#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<BasicSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  // clang-format off
  vector<vector<char>> board{
    {'A', 'B', 'C', 'E'},
    {'S', 'F', 'C', 'S'},
    {'A', 'D', 'E', 'E'}
  };
  // clang-format on
  string word = "ABCCED";
  EXPECT_EQ(this->getSolution().exist(board, word), true);
}

TYPED_TEST(SolutionTest, Test2) {
  // clang-format off
  vector<vector<char>> board{
    {'A','B','C','E'},
    {'S','F','C','S'},
    {'A','D','E','E'}
  };
  // clang-format on
  string word = "SEE";
  EXPECT_EQ(this->getSolution().exist(board, word), true);
}

TYPED_TEST(SolutionTest, Test3) {
  // clang-format off
  vector<vector<char>> board{
    {'A','B','C','E'},
    {'S','F','C','S'},
    {'A','D','E','E'}
  };
  // clang-format on
  string word = "ABCB";
  EXPECT_EQ(this->getSolution().exist(board, word), false);
}

TYPED_TEST(SolutionTest, Test4) {
  // clang-format off
  vector<vector<char>> board{
    {'A','B','C','E'},
    {'S','F','C','S'},
    {'A','D','E','E'}
  };
  // clang-format on
  string word = "D";
  EXPECT_EQ(this->getSolution().exist(board, word), true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
