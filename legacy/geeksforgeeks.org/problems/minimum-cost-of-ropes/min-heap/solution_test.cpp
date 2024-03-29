#include "solution.hpp"
#include <gtest/gtest.h>

using Int = long long;

#define TEST_SOLUTION()                                                        \
  {                                                                            \
    Solution s;                                                                \
    SCOPED_TRACE("min heap");                                                  \
    Int n = sizeof(arr) / sizeof(long long);                                   \
    Int actual = s.minCost(arr, n, SolutionKind::SK_MinHeap);                  \
    EXPECT_EQ(expected, actual);                                               \
  }                                                                            \
  {                                                                            \
    Solution s;                                                                \
    SCOPED_TRACE("map queue");                                                 \
    Int n = sizeof(arr) / sizeof(long long);                                   \
    Int actual = s.minCost(arr, n, SolutionKind::SK_MapPQ);                    \
    EXPECT_EQ(expected, actual);                                               \
  }                                                                            \
  {                                                                            \
    Solution s;                                                                \
    SCOPED_TRACE("std heap");                                                  \
    Int n = sizeof(arr) / sizeof(long long);                                   \
    Int actual = s.minCost(arr, n, SolutionKind::SK_STDMinHeap);               \
    EXPECT_EQ(expected, actual);                                               \
  }

TEST(SolutionTest, Example1) {
  // 2, 3, 4, 6 -> 5, 4, 6 -> 5 + 9, 6 -> 5 + 9 + 15 = 29
  Int arr[] = {4, 3, 2, 6};
  const Int expected = 29;
  TEST_SOLUTION();
}

TEST(SolutionTest, Example2) {
  Int arr[] = {10, 20, 30};
  Int expected = 90;
  // {10+20, 30}, cost: 30
  // {30+30},     cost: 60
  // {60},        total: 30+60=90
  TEST_SOLUTION();
}

TEST(SolutionTest, Example3) {
  Int arr[] = {9, 5, 7, 11};
  Int expected = 64;
  // {5+7, 9, 11},  cost: 12
  // {11+9, 12},    cost: 20
  // {20+12},       cost: 32
  // {32},          tota: 12+20+32 = 64
  TEST_SOLUTION();
}

TEST(SolutionTest, Example4) {
  Int arr[] = {1, 3, 2, 4};
  Int expected = 19;
  TEST_SOLUTION();
}

TEST(SolutionTest, Example5) {
  Int arr[] = {16, 8, 12};
  Int expected = 56;
  // {8+12, 16},    cost: 20
  // {20+16},       cost: 36
  // 36,            total: 36+20 = 5
  TEST_SOLUTION();
}

TEST(SolutionTest, GFG_1) {
  Int arr[] = {4, 2, 7, 6, 9};
  Int expected = 62;
  TEST_SOLUTION();
}

TEST(SolutionTest, Long) {
  Int arr[] = {999, 66, 127, 5, 7, 999, 9, 4789, 11, 8};
  Int expected = 10914;
  TEST_SOLUTION();
}
