#include <gtest/gtest.h>
#include <vector>

using namespace std;

class Solution {
  // too slow, exceeds the time limit
  [[nodiscard]] int bruteForce(const vector<int> &heights) const noexcept {
    const int size = heights.size();
    int maxArea = 0;
    for (int left = 0; left < size; ++left) {
      for (int right = left + 1; right < size; ++right) {
        const int minHeight = min(heights[left], heights[right]);
        const int width = right - left;
        maxArea = max(maxArea, (width * minHeight));
      }
    }
    return maxArea;
  }

  [[nodiscard]] int twoPointers(const vector<int> &heights) const noexcept {}

public:
  int maxArea(vector<int> &heights) { return twoPointers(heights); }
};

TEST(ContainerWithMostWaterTest, LeetCodeExample1) {
  vector<int> height{1, 8, 6, 2, 5, 4, 8, 3, 7};
  EXPECT_EQ(Solution().maxArea(height), 49);
}