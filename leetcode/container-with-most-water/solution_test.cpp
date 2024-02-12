#include <gtest/gtest.h>
#include <vector>

using namespace std;

class Solution {
  [[nodiscard]] int twoPointers(const vector<int> &heights) const noexcept {
    int l = 0;
    int r = heights.size() - 1;
    int maxArea = 0;
    while (l < r) {
      int area = (r - l) * min(heights[l], heights[r]);
      maxArea = max(area, maxArea);
      if (heights[l] == heights[r])
        ++l, --r;
      else if (heights[l] < heights[r])
        ++l;
      else
        --r;
    }
    return maxArea;
  }

public:
  int maxArea(vector<int> &heights) { return twoPointers(heights); }
};

TEST(ContainerWithMostWaterTest, LeetCodeExample1) {
  vector<int> height{1, 8, 6, 2, 5, 4, 8, 3, 7};
  EXPECT_EQ(Solution().maxArea(height), 49);
}