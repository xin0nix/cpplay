#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

class Solution {
public:
  bool mergeTriplets(const std::vector<std::vector<int>> &triplets,
                     const std::vector<int> &target) {
    std::array<bool, 3> match = {false};
    for (const auto &triplet : triplets) {
      if (triplet.at(0) > target.at(0) or triplet.at(1) > target.at(1) or
          triplet.at(2) > target.at(2)) {
        continue;
      }
      for (auto pos = 0UL; pos < 3; ++pos) {
        if (triplet.at(pos) == target.at(pos)) {
          match.at(pos) = true;
        }
      }
    }
    return std::all_of(match.begin(), match.end(),
                       [](bool val) { return val; });
  }
};

// Declaration of the function to be tested
bool mergeTriplets(const std::vector<std::vector<int>> &triplets,
                   const std::vector<int> &target) {
  return Solution().mergeTriplets(triplets, target);
}

// Struct to hold each test case's input and expected result
struct MergeTripletsTestCase {
  std::vector<std::vector<int>> triplets;
  std::vector<int> target;
  bool expected;
};

// Parameterized test fixture
class MergeTripletsTest
    : public ::testing::TestWithParam<MergeTripletsTestCase> {};

TEST_P(MergeTripletsTest, HandlesVariousCases) {
  const auto &param = GetParam();
  EXPECT_EQ(mergeTriplets(param.triplets, param.target), param.expected);
}

// Test cases
INSTANTIATE_TEST_SUITE_P(
    MergeTripletsTests, MergeTripletsTest,
    ::testing::Values(
        // Example 1: Can form the target
        MergeTripletsTestCase{
            {{2, 5, 3}, {1, 8, 4}, {1, 7, 5}}, {2, 7, 5}, true},
        // Example 2: Cannot form the target
        MergeTripletsTestCase{{{3, 4, 5}, {4, 5, 6}}, {3, 2, 5}, false},
        // Example 3: Target already present
        MergeTripletsTestCase{{{2, 7, 5}}, {2, 7, 5}, true},
        // Example 4: No triplet can be merged to target
        MergeTripletsTestCase{{{1, 3, 4}, {2, 5, 8}}, {2, 5, 7}, false},
        // Example 5: Multiple triplets needed to form target
        MergeTripletsTestCase{
            {{1, 2, 3}, {2, 3, 4}, {2, 7, 5}}, {2, 7, 5}, true},
        // Example 6: Empty triplets
        MergeTripletsTestCase{{}, {1, 2, 3}, false},
        // Example 7: Triplets with all zeros, target is zero
        MergeTripletsTestCase{{{0, 0, 0}}, {0, 0, 0}, true}));
