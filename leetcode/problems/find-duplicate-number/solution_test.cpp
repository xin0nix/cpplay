#include <gtest/gtest.h>
#include <vector>

using namespace std;

struct BruteForce {
  int findDuplicate(vector<int> &nums) {
    int slow = 0, fast = 0;
    do {
      slow = nums[slow];
      fast = nums[nums[fast]];
    } while (slow != fast);
    int head = 0;
    while (head != slow) {
      slow = nums[slow];
      head = nums[head];
    }
    return slow;
  }
};

TEST(FindDuplicateTest, LeetCode1) {
  vector nums{1, 3, 4, 2, 2};
  int n = BruteForce().findDuplicate(nums);
  ASSERT_EQ(n, 2);
}

TEST(FindDuplicateTest, LeetCode2) {
  vector nums{3, 1, 3, 4, 2};
  int n = BruteForce().findDuplicate(nums);
  ASSERT_EQ(n, 3);
}

TEST(FindDuplicateTest, LeetCode3) {
  vector nums{3, 3, 3, 3, 3};
  int n = BruteForce().findDuplicate(nums);
  ASSERT_EQ(n, 3);
}

TEST(FindDuplicateTest, LeetCodeBug) {
  vector nums{2, 5, 9, 6, 9, 3, 8, 9, 7, 1};
  int n = BruteForce().findDuplicate(nums);
  ASSERT_EQ(n, 9);
}
