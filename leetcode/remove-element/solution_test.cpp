#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

using std::vector;
using testing::UnorderedElementsAre;

struct Solution {
  std::size_t removeElement(vector<int> &nums, const int val) {
    auto j = nums.begin();
    for (const int n : nums) {
      if (n != val) {
        *j = n;
        ++j;
      }
    }
    return std::distance(nums.begin(), j);
  }
};

TEST(RemoveElement, Leet) {
  {
    SCOPED_TRACE("Case 1");
    vector nums{3, 2, 2, 3};
    const size_t s = Solution().removeElement(nums, 3);
    ASSERT_TRUE(s <= nums.size());
    vector<int> res(nums.cbegin(), nums.cbegin() + s);
    EXPECT_THAT(res, UnorderedElementsAre(2, 2));
  }
  {
    SCOPED_TRACE("Case 2");
    vector nums{0, 1, 2, 2, 3, 0, 4, 2};
    const size_t s = Solution().removeElement(nums, 2);
    ASSERT_TRUE(s <= nums.size());
    vector<int> res(nums.cbegin(), nums.cbegin() + s);
    EXPECT_THAT(res, UnorderedElementsAre(0, 1, 3, 0, 4));
  }
}
