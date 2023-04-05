#include "solution.hpp"
#include <gtest/gtest.h>

TEST(SolutionTest, Test1) {
  Solution solution;
  EXPECT_EQ(solution.solve(), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
