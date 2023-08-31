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
  int price[] = {1};
  EXPECT_EQ(this->getSolution().cutRod(price, 0), 0);
}

TYPED_TEST(SolutionTest, Test2) {
  int price[] = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
  EXPECT_EQ(this->getSolution().cutRod(price, 0), 0);
  EXPECT_EQ(this->getSolution().cutRod(price, 1), 1);
  EXPECT_EQ(this->getSolution().cutRod(price, 2), 5);
  EXPECT_EQ(this->getSolution().cutRod(price, 3), 8);
  EXPECT_EQ(this->getSolution().cutRod(price, 4), 10);
  EXPECT_EQ(this->getSolution().cutRod(price, 5), 13);
  EXPECT_EQ(this->getSolution().cutRod(price, 6), 17);
  EXPECT_EQ(this->getSolution().cutRod(price, 7), 18);
  EXPECT_EQ(this->getSolution().cutRod(price, 8), 22);
  EXPECT_EQ(this->getSolution().cutRod(price, 9), 25);
  EXPECT_EQ(this->getSolution().cutRod(price, 10), 30);
}

TYPED_TEST(SolutionTest, Test3) {
  int price[] = {1, 5, 8, 9, 10, 17, 17, 20};
  EXPECT_EQ(this->getSolution().cutRod(price, 8), 22);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
