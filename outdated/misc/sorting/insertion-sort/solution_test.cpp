#include "solution.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(SolutionTest, Test1) {
  Solution solution;
  std::vector<int> numbers{10, 0, -7, 3, 2, 1, 4};
  solution.sort(numbers);
  // TODO: the logic
  EXPECT_THAT(numbers, testing::ElementsAre(-7, 0, 1, 2, 3, 4, 10));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
