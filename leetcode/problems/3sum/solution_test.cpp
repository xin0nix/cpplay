#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

using namespace std;
using namespace testing;

class BruteForce {
  vector<vector<int>> res;

  inline void twoSum(vector<int> &nums, int k, int left, int right) {
    while (left < right) {
      const int sum = nums[k] + nums[left] + nums[right];
      if (sum == 0) {
        res.push_back({nums[k], nums[left], nums[right]});
        ++left;
        while (left < right && nums[left] == nums[left - 1])
          ++left;
      } else if (sum < 0) {
        ++left;
      } else {
        --right;
      }
    }
  }

public:
  vector<vector<int>> threeSum(vector<int> &nums) {
    int size = static_cast<int>(nums.size());
    sort(nums.begin(), nums.end());
    for (int k = 0; k < size - 2; ++k) {
      if (k > 0 && nums[k] == nums[k - 1])
        continue;
      twoSum(nums, k, k + 1, size - 1);
    }
    return res;
  }
};

TEST(ThreeSumTest, LeetCodeExample1) {
  // -4, -1, -1, 0, 1, 2
  vector<int> nums{-1, 0, 1, 2, -1, -4};
  auto res = BruteForce().threeSum(nums);
  vector<string> converted;
  ranges::transform(res.begin(), res.end(), back_inserter(converted),
                    [](auto abc) {
                      return "FIXME";
                      // return fmt::format("[{},{},{}]", abc[0], abc[1], abc[2]);
                    });
  EXPECT_THAT(converted, UnorderedElementsAre("[-1,-1,2]", "[-1,0,1]"));
}

TEST(ThreeSumTest, LeetCodeExample2) {
  vector<int> nums{0, 0, 1};
  auto res = BruteForce().threeSum(nums);
  vector<string> converted;
  ranges::transform(res.begin(), res.end(), back_inserter(converted),
                    [](auto abc) {
                      return "FIXME";
                      // return fmt::format("[{},{},{}]", abc[0], abc[1], abc[2]);
                    });
  EXPECT_THAT(converted, UnorderedElementsAre());
}

TEST(ThreeSumTest, LeetCodeExample3) {
  vector<int> nums{0, 0, 0};
  auto res = BruteForce().threeSum(nums);
  vector<string> converted;
  ranges::transform(res.begin(), res.end(), back_inserter(converted),
                    [](auto abc) {
                      return "FIXME";
                      // return fmt::format("[{},{},{}]", abc[0], abc[1], abc[2]);
                    });
  EXPECT_THAT(converted, UnorderedElementsAre("[0,0,0]"));
}
