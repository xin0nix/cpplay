#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <concepts>
#include <deque>
#include <iterator>
#include <list>
#include <vector>

// Problem link: https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/
/*
Problem statement:
Given a 1-indexed array of integers numbers that is already sorted in
non-decreasing order, find two numbers such that they add up to a specific
target number. Let these two numbers be numbers[index1] and numbers[index2]
where 1 <= index1 < index2 <= numbers.length.

Return the indices of the two numbers, index1 and index2, added by one as an
integer array [index1, index2] of length 2.

The tests are generated such that there is exactly one solution. You may not use
the same element twice.
*/

template <typename T>
concept SolutionFrendlyContainer = requires(T a) {
  { a.begin() } -> std::forward_iterator<>;
  { a.rbegin() } -> std::forward_iterator<>;
};

template <SolutionFrendlyContainer C> struct SolutionImpl {
  using R = typename C::value_type;
  C twoSum(const C &numbers, R target) {
    C result;
    auto l = numbers.begin();
    auto r = numbers.rbegin();
    while (true) {
      R sum = *l + *r;
      if (sum == target)
        break;
      else if (sum > target)
        r++;
      else
        l++;
    }
    return {
        static_cast<R>(std::distance(numbers.begin(), l) + 1UL),
        static_cast<R>(numbers.size() - std::distance(numbers.rbegin(), r))};
  }
};

class Solution {
public:
  std::vector<int> twoSum(std::vector<int> &numbers, int target) {
    return SolutionImpl<std::vector<int>>().twoSum(numbers, target);
  }
};

template <SolutionFrendlyContainer C>
struct SolutionTest : public testing::Test {
  using S = SolutionImpl<C>;
  S solution;
  S &getSolution() { return solution; }
};

using Implementations =
    testing::Types<std::vector<int>, std::list<int>, std::deque<int>>;

TYPED_TEST_SUITE(SolutionTest, Implementations);

TYPED_TEST(SolutionTest, NeetCodeVectorTest) {
  ASSERT_THAT(this->getSolution().twoSum({1, 3, 4, 5, 7, 11}, 9),
              testing::ElementsAre(3, 4));
}

TYPED_TEST(SolutionTest, LeetCodeA) {
  ASSERT_THAT(this->getSolution().twoSum({2, 7, 11, 15}, 9),
              testing::ElementsAre(1, 2));
}

TYPED_TEST(SolutionTest, LeetCodeB) {
  ASSERT_THAT(this->getSolution().twoSum({2, 3, 4}, 6),
              testing::ElementsAre(1, 3));
}

TYPED_TEST(SolutionTest, LeetCodeC) {
  ASSERT_THAT(this->getSolution().twoSum({-1, 0}, -1),
              testing::ElementsAre(1, 2));
}
