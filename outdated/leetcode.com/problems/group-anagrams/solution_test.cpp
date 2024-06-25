#include "solution.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::Types;

template <class T> struct SolutionTest : public testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

typedef Types<MapSolution> Implementations;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, Test1) {
  std::vector<std::string> words{"aba", "baa", "bab", "aab"};
  Solution s;
  auto r = s.groupAnagrams(words);
  EXPECT_THAT(r, testing::Contains(testing::UnorderedElementsAre("bab")));
  EXPECT_THAT(
      r, testing::Contains(testing::UnorderedElementsAre("aab", "baa", "aba")));
}

TYPED_TEST(SolutionTest, Test2) {
  std::vector<std::string> words{"eat", "tea", "tan", "ate", "nat", "bat"};
  Solution s;
  auto r = s.groupAnagrams(words);
  EXPECT_THAT(r, testing::Contains(testing::UnorderedElementsAre("bat")));
  EXPECT_THAT(r,
              testing::Contains(testing::UnorderedElementsAre("nat", "tan")));
  EXPECT_THAT(
      r, testing::Contains(testing::UnorderedElementsAre("ate", "tea", "eat")));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
