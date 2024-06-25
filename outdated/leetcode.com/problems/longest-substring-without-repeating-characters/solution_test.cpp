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
  EXPECT_EQ(this->getSolution().lengthOfLongestSubstring(""), 0);
}

TYPED_TEST(SolutionTest, Test2) {
  EXPECT_EQ(this->getSolution().lengthOfLongestSubstring("abc"), 3);
}

TYPED_TEST(SolutionTest, Test3) {
  EXPECT_EQ(this->getSolution().lengthOfLongestSubstring("abba"), 2);
}

TYPED_TEST(SolutionTest, Test4) {
  EXPECT_EQ(this->getSolution().lengthOfLongestSubstring(
                "ababcabcdabaabbcdddabcdefaaffeebc"),
            6);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
