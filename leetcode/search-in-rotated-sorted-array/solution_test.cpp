#include <gtest/gtest.h>
#include <vector>

using namespace std;

class Solution {
public:
  [[nodiscard]] int search(const vector<int> &nums,
                           const int target) const noexcept {
    for (int l = 0, r = nums.size() - 1; l <= r;) {
      int m = (l + r) / 2;
      // if we found solution - great!
      if (nums[m] == target) [[unlikely]]
        return m;
      // keep searching
      if (nums[l] <= nums[m]) {
        // the target is in a contigous sorted left interval
        if (nums[l] <= target && target <= nums[m])
          r = m - 1;
        else
          l = m + 1;
      } else {
        // the target is in a contiguous sorted right interval
        if (nums[m] <= target && target <= nums[r])
          l = m + 1;
        else
          r = m - 1;
      }
    }
    return -1;
  }
};

TEST(SearchInRotatedSortedArray, LeetCodeExample1) {
  vector nums{4, 5, 6, 7, 0, 1, 2};
  Solution s;
  EXPECT_EQ(s.search(nums, 4), 0);
  EXPECT_EQ(s.search(nums, 7), 3);
  EXPECT_EQ(s.search(nums, 0), 4);
  EXPECT_EQ(s.search(nums, 2), 6);
  // missing
  EXPECT_EQ(s.search(nums, 3), -1);
  EXPECT_EQ(s.search(nums, 8), -1);
  EXPECT_EQ(s.search(nums, -1), -1);
}

TEST(SearchInRotatedSortedArray, Empty) {
  vector<int> nums{};
  Solution s;
  EXPECT_EQ(s.search(nums, 42), -1);
}

TEST(SearchInRotatedSortedArray, Trivial123) {
  vector nums{1, 2, 3};
  Solution s;
  EXPECT_EQ(s.search(nums, 0), -1);
  EXPECT_EQ(s.search(nums, 1), 0);
  EXPECT_EQ(s.search(nums, 2), 1);
  EXPECT_EQ(s.search(nums, 3), 2);
  EXPECT_EQ(s.search(nums, 4), -1);
}
