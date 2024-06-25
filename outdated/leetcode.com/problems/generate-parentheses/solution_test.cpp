#include "solution.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<BactrackingSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test0) {
  int n = 0;
  auto r = this->getSolution().generateParenthesis(n);
  EXPECT_THAT(r, testing::UnorderedElementsAre(""));
}

TYPED_TEST(SolutionTest, Test1) {
  int n = 1;
  auto r = this->getSolution().generateParenthesis(n);
  EXPECT_THAT(r, testing::UnorderedElementsAre("()"));
}

TYPED_TEST(SolutionTest, Test2) {
  int n = 2;
  auto r = this->getSolution().generateParenthesis(n);
  EXPECT_THAT(r, testing::UnorderedElementsAre("()()", "(())"));
}

TYPED_TEST(SolutionTest, Test3) {
  int n = 3;
  auto r = this->getSolution().generateParenthesis(n);
  EXPECT_THAT(r, testing::UnorderedElementsAre("()()()", "()(())", "(())()",
                                               "(()())", "((()))"));
}

TYPED_TEST(SolutionTest, Test4) {
  int n = 4;
  auto r = this->getSolution().generateParenthesis(n);
  EXPECT_THAT(r, testing::UnorderedElementsAre(
                     "(((())))", "((()()))", "((())())", "((()))()", "(()(()))",
                     "(()()())", "(()())()", "(())(())", "(())()()", "()((()))",
                     "()(()())", "()(())()", "()()(())", "()()()()"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
