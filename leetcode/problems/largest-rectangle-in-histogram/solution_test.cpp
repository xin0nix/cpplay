#include <gtest/gtest.h>
#include <stack>
#include <vector>

#include <ranges>
class Solution {
public:
  int largestRectangleArea(const std::vector<int> &heights) const {
    struct Frame {
      int startPos;
      int height;
      int getArea(int pos) const { return (pos - startPos) * height; }
    };
    std::stack<Frame> frames;
    frames.push({.startPos = 0, .height = heights.at(0)});
    int maxArea{0};
    int len = heights.size();
    for (int pos = 1; pos <= len; ++pos) {
      int curHeight = (pos < len) ? heights.at(pos) : 0;
      int prevPos = pos;
      while (!frames.empty() && curHeight < frames.top().height) {
        maxArea = std::max(maxArea, frames.top().getArea(pos));
        prevPos = frames.top().startPos;
        frames.pop();
      }
      if (pos < len) {
        frames.push({.startPos = prevPos, .height = curHeight});
      }
    }

    while (not frames.empty()) {
      maxArea = std::max(maxArea, frames.top().getArea(heights.size()));
      frames.pop();
    }

    return maxArea;
  }
};

// Define a struct for test parameters to improve readability
struct LargestRectangleTestParams {
  std::vector<int> heights;
  int expectedArea;
  std::string test_name;
};

// Test fixture for parametrized tests
class LargestRectangleInHistogramTest
    : public ::testing::TestWithParam<LargestRectangleTestParams> {};

// Parametrized test case
TEST_P(LargestRectangleInHistogramTest, CalculatesCorrectArea) {
  auto param = GetParam();
  Solution solution;
  // The input vector can be modified by some solutions, so we make a copy.
  std::vector<int> heights_copy = param.heights;
  EXPECT_EQ(solution.largestRectangleArea(heights_copy), param.expectedArea);
}

// Instantiate the test suite with a set of test cases
INSTANTIATE_TEST_SUITE_P(
    DefaultCases, LargestRectangleInHistogramTest,
    ::testing::Values(
        LargestRectangleTestParams{{2, 1, 5, 6, 2, 3}, 10, "LeetCodeExample1"},
        LargestRectangleTestParams{{2, 4}, 4, "LeetCodeExample2"},
        LargestRectangleTestParams{{1}, 1, "SingleBar"},
        LargestRectangleTestParams{{5, 5, 5, 5}, 20, "AllBarsSameHeight"},
        LargestRectangleTestParams{{0, 0, 0}, 0, "AllZeroHeight"},
        LargestRectangleTestParams{
            {1, 2, 3, 4, 5}, 9, "StrictlyIncreasingHeights"},
        LargestRectangleTestParams{
            {5, 4, 3, 2, 1}, 9, "StrictlyDecreasingHeights"},
        LargestRectangleTestParams{{1, 5, 1}, 5, "MountainShape"},
        LargestRectangleTestParams{{5, 1, 5}, 5, "ValleyShape"},
        LargestRectangleTestParams{{2, 1, 0, 5, 6}, 10, "HistogramWithZero"},
        LargestRectangleTestParams{{2, 1, 2}, 3, "SmallValley"},
        LargestRectangleTestParams{
            {6, 2, 5, 4, 5, 1, 6}, 12, "MultiplePeaksAndValleys"}),
    // Lambda to generate descriptive test names
    [](const testing::TestParamInfo<LargestRectangleTestParams> &info) {
      return info.param.test_name;
    });
