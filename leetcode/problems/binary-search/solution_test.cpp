
#include <algorithm>
#include <concepts>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iterator>
#include <type_traits>
#include <vector>

using namespace std;
using namespace ::testing;

template <typename RandomAccessIter>
requires random_access_iterator<RandomAccessIter>
static int binarySearchImpl(RandomAccessIter first, RandomAccessIter last,
                            typename RandomAccessIter::value_type target) {
  RandomAccessIter left = first, right = last - 1;
  while (distance(left, right) >= 0) {
    RandomAccessIter middle = left + distance(left, right) / 2;
    if (target == *middle)
      return distance(first, middle);
    else if (target < *middle)
      right = middle - 1;
    else
      left = middle + 1;
  }
  return -1;
}

// FIXME: iterator-based implementation
struct NaiveSolution {
  int search(const vector<int> &nums, const int target) {
    return binarySearchImpl(nums.begin(), nums.end(), target);
  }
};

struct LowerBoundSolution {
  int search(const vector<int> &nums, const int target) {
    auto bound = lower_bound(nums.begin(), nums.end(), target);
    if (bound == nums.end())
      return -1;
    if (*bound != target)
      return -1;
    return distance(nums.begin(), bound);
  }
};

using SolutionTypes = Types<NaiveSolution, LowerBoundSolution>;

// FIXME:
// - take not only vectors, but anything vector-ish, for example array
// - allow for any integral type
template <typename T>
concept BinarySearchSolution = requires(T a) {
  { a.search(declval<vector<int>>(), declval<int>()) } -> convertible_to<int>;
};

template <BinarySearchSolution T> struct BinarySearchTest : Test {
  T solution;
  T &getSolution() { return solution; }
};

TYPED_TEST_SUITE(BinarySearchTest, SolutionTypes);

TYPED_TEST(BinarySearchTest, LeetCodeExample1) {
  vector nums({-1, 0, 3, 5, 9, 12});
  ASSERT_EQ(this->getSolution().search(nums, 9), 4);
}

TYPED_TEST(BinarySearchTest, LeetCodeExample2) {
  vector nums({-1, 0, 3, 5, 9, 12});
  ASSERT_EQ(this->getSolution().search(nums, 2), -1);
}

TYPED_TEST(BinarySearchTest, LeetCodeExample3) {
  vector nums({-1, 0, 3, 5, 9, 12});
  ASSERT_EQ(this->getSolution().search(nums, 3), 2);
}

TYPED_TEST(BinarySearchTest, MyExample1) {
  vector nums({-1, 0, 3, 5, 9, 12});
  ASSERT_EQ(this->getSolution().search(nums, 333), -1);
}

TYPED_TEST(BinarySearchTest, MyExample2) {
  vector nums({-1, 0, 3, 5, 9, 12});
  ASSERT_EQ(this->getSolution().search(nums, -100), -1);
}

TYPED_TEST(BinarySearchTest, LeetCodeBug1) {
  vector nums({-1});
  ASSERT_EQ(this->getSolution().search(nums, 2), -1);
}
