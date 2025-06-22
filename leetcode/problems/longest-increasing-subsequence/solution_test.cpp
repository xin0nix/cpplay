#include <gtest/gtest.h>
#include <vector>
using namespace std;

std::ostream &operator<<(std::ostream &os, std::span<const int> nums) {
  os << "| ";
  for (auto n : nums) {
    os << n << " | ";
  }
  return os;
}

class Solution {
public:
  int lengthOfLIS(const vector<int> &nums) {
    std::vector lis(nums.size(), 1);
    int answer = 1;
    for (int start = int(nums.size()) - 1; start >= 0; --start) {
      int maxLis = 1;
      for (int pos = start + 1; pos < int(nums.size()); ++pos) {
        if (nums.at(start) < nums.at(pos)) {
          maxLis = std::max(maxLis, lis.at(pos) + 1);
        }
      }
      lis[start] = maxLis;
      answer = std::max(answer, maxLis);
    }
    return answer;
  }
};

// Define parameter structure
struct TestCase {
  vector<int> nums;
  int expected;
};

// Create parameterized test class
class LISTest : public testing::TestWithParam<TestCase> {
protected:
  Solution solution;
};

TEST_P(LISTest, HandlesAllCases) {
  auto params = GetParam();
  vector<int> nums = params.nums;
  int result = solution.lengthOfLIS(nums);
  ASSERT_EQ(result, params.expected);
}

// Instantiate test cases
INSTANTIATE_TEST_SUITE_P(
    AllTestCases, LISTest,
    testing::Values(
        // Corner cases
        TestCase{{5}, 1},             // Single element
        TestCase{{5, 4, 3, 2, 1}, 1}, // Strictly decreasing
        TestCase{{2, 2, 2, 2}, 1},    // All duplicates

        // Standard cases
        TestCase{{10, 9, 2, 5, 3, 7, 101, 18}, 4},      // LeetCode example
        TestCase{{0, 8, 4, 12, 2, 10, 6, 14, 1, 9}, 4}, // Multiple valid LIS
        TestCase{{1, 3, 6, 7, 9, 4, 10, 5, 6}, 6},      // Increasing with dip

        // Complex cases
        TestCase{{3, 10, 2, 1, 20}, 3},      // Non-adjacent elements
        TestCase{{50, 3, 10, 7, 40, 80}, 4}, // Multiple increasing subsequences
        TestCase{{10, 22, 9, 33, 21, 50, 41, 60}, 5}, // Standard sequence

        // Negative numbers
        TestCase{{-5, -4, -3, -2, -1}, 5}, // Increasing negatives
        TestCase{{-1, -2, -3, -4}, 1},     // Decreasing negatives
        TestCase{{-2, -1, 0, -5, -4}, 3}   // Mixed negatives
        ));
