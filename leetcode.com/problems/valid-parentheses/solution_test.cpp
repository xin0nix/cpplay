#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<StackSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::string str = ")";
  EXPECT_EQ(this->getSolution().isValid(str), false);
}

TYPED_TEST(SolutionTest, Test2) {
  std::string str = "[(])";
  EXPECT_EQ(this->getSolution().isValid(str), false);
}

TYPED_TEST(SolutionTest, Test3) {
  std::string str = "()[]{}";
  EXPECT_EQ(this->getSolution().isValid(str), true);
}

TYPED_TEST(SolutionTest, Test4) {
  std::string str = "([{}])[{()}]{([({})])}";
  EXPECT_EQ(this->getSolution().isValid(str), true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
