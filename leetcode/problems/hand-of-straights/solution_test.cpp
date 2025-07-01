#include <gtest/gtest.h>

#include <queue>
#include <unordered_map>
#include <vector>

class Solution {
public:
  bool isNStraightHand(const std::vector<int> &hand, const int groupSize) {
    if (hand.size() % groupSize != 0) {
      return false;
    }
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap{
        std::greater<int>{}};
    std::unordered_map<int, int> hashMap;
    for (auto straight : hand) {
      hashMap[straight]++;
    }
    for (auto &&[key, value] : hashMap) {
      minHeap.push(key);
    }
    while (!minHeap.empty()) {
      int base = minHeap.top();
      if (hashMap.at(base) == 0) {
        minHeap.pop();
        continue;
      }
      for (int idx = base; idx < base + groupSize; ++idx) {
        auto it = hashMap.find(idx);
        if (it == hashMap.end()) {
          return false;
        }
        if (it->second == 0) {
          return false;
        }
        --it->second;
      }
    }
    return true;
  }
};

// Declaration of the function to be tested
bool isNStraightHand(std::vector<int> &hand, int groupSize) {
  return Solution().isNStraightHand(hand, groupSize);
}

// Struct to hold each test case's input and expected output
struct HandOfStraightsTestCase {
  std::vector<int> hand;
  int groupSize;
  bool expected;
};

// Parameterized test fixture
class HandOfStraightsTest
    : public ::testing::TestWithParam<HandOfStraightsTestCase> {};

TEST_P(HandOfStraightsTest, HandlesVariousCases) {
  HandOfStraightsTestCase tc = GetParam();
  std::vector<int> handCopy =
      tc.hand; // Defensive copy in case function modifies input
  bool result = isNStraightHand(handCopy, tc.groupSize);
  EXPECT_EQ(result, tc.expected);
}

// Test cases based on problem description and edge cases
INSTANTIATE_TEST_SUITE_P(
    HandOfStraightsTests, HandOfStraightsTest,
    ::testing::Values(
        // Example 1: Can be rearranged into groups
        HandOfStraightsTestCase{{1, 2, 3, 6, 2, 3, 4, 7, 8}, 3, true},
        // Example 2: Cannot be rearranged into groups
        HandOfStraightsTestCase{{1, 2, 3, 4, 5}, 4, false},
        // Single group, should succeed
        HandOfStraightsTestCase{{1, 2, 3}, 3, true},
        // All cards the same, groupSize > 1, should fail
        HandOfStraightsTestCase{{2, 2, 2, 2}, 2, false},
        // Large groupSize, not enough cards
        HandOfStraightsTestCase{{1, 2, 3}, 4, false},
        // Edge case: groupSize == 1, always true
        HandOfStraightsTestCase{{10, 20, 30}, 1, true},
        // Edge case: empty hand (not allowed by constraints, but for
        // robustness)
        HandOfStraightsTestCase{{}, 1, true},
        // Edge case: groupSize equals hand size, consecutive
        HandOfStraightsTestCase{{5, 6, 7, 8}, 4, true},
        // Edge case: groupSize equals hand size, not consecutive
        HandOfStraightsTestCase{{1, 3, 5, 7}, 4, false}));
