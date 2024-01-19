#include "solution.hpp"
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<BruteForceSolution, TwoPointerSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::string phrase = " ";
  EXPECT_EQ(this->getSolution().isPalindrome(phrase), true);
}

TYPED_TEST(SolutionTest, Test2) {
  std::string phrase = "A man, a plan, a canal: Panama";
  EXPECT_EQ(this->getSolution().isPalindrome(phrase), true);
}

TYPED_TEST(SolutionTest, Test3) {
  std::string phrase = "race a car";
  EXPECT_EQ(this->getSolution().isPalindrome(phrase), false);
}

TYPED_TEST(SolutionTest, Test4) {
  std::string phrase = "a.";
  EXPECT_EQ(this->getSolution().isPalindrome(phrase), true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
