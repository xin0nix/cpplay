
#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

using namespace std;
using namespace ::testing;

struct NaiveSolution {
  int search(const vector<int> &nums, const int target) {
    const int len = static_cast<int>(nums.size());
    int idx = -1;
    int l = 0, r = len - 1;
    while (l <= r) {
      int m = l + ((r - l) / 2);
      const int val = nums[m];
      if (target == val)
        return m;
      else if (target < val)
        r = m - 1; // l .. target? .. m-1
      else
        l = m + 1; // m+1 .. target? .. r
    }
    return idx;
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

template <typename T> struct BinarySearchTest : Test {
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
