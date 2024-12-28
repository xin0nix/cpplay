#include <gtest/gtest.h>
#include <vector>

using namespace std;

class BruteForce {
public:
  int findMin(vector<int> &nums) {
    int l = 0;
    int r = nums.size() - 1;
    int res = nums[l];
    while (l <= r) {
      // Trivial case - this portion of array is fully sorted
      if (nums[l] <= nums[r]) {
        res = min(res, nums[l]);
        break;
      }
      // l ... m .. r
      int m = (l + r) / 2;
      res = min(res, nums[m]);
      // The middle is in the left (fully sorted, but rotated) portion, search
      // right: nums[r] is smaller than nums[l]
      if (nums[l] <= nums[m])
        l = m + 1;
      else
        r = m - 1;
    }
    return res;
  }
};

TEST(FindMinRotatedTest, LeetCode1) {
  vector nums{3, 4, 5, 1, 2};
  EXPECT_EQ(BruteForce().findMin(nums), 1);
}

TEST(FindMinRotatedTest, LeetCode2) {
  vector nums{4, 5, 6, 7, 0, 1, 2};
  EXPECT_EQ(BruteForce().findMin(nums), 0);
}

TEST(FindMinRotatedTest, LeetCode3) {
  vector nums{11, 13, 15, 17};
  EXPECT_EQ(BruteForce().findMin(nums), 11);
}

TEST(FindMinRotatedTest, Custom1) {
  vector nums{11, 13, 15, 17, 10};
  EXPECT_EQ(BruteForce().findMin(nums), 10);
}

TEST(FindMinRotatedTest, LeetCodeBug1) {
  vector nums{1, 2};
  EXPECT_EQ(BruteForce().findMin(nums), 1);
}

TEST(FindMinRotatedTest, LeetCodeBug2) {
  vector nums{3, 1, 2};
  EXPECT_EQ(BruteForce().findMin(nums), 1);
}

TEST(FindMinRotatedTest, LeetCodeBug3) {
  vector nums{5, 1, 2, 3, 4};
  EXPECT_EQ(BruteForce().findMin(nums), 1);
}
