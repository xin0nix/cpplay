#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <vector>
using std::vector;

#include <ranges>
struct Solution {
  static constexpr auto bitsIn = [](uint32_t target) {
    int count = 0;
    for (int i = 0; i < 32; i++, target >>= 1) {
      if ((target & 0x01) == 1) {
        count++;
      }
    }
    return count;
  };

  std::vector<int> countBits(int32_t bound) {
    return std::views::iota(0, bound + 1) | std::views::transform(bitsIn) |
           std::ranges::to<std::vector>();
  }
};

struct CountingBitsTestCase {
  int n;
  vector<int> expected;
};

class CountingBitsTest : public ::testing::TestWithParam<CountingBitsTestCase> {
};

TEST_P(CountingBitsTest, HandlesVariousInputs) {
  const auto &param = GetParam();
  Solution sol;
  EXPECT_THAT(sol.countBits(param.n),
              ::testing::ElementsAreArray(param.expected));
}

INSTANTIATE_TEST_SUITE_P(
    CountingBitsTests, CountingBitsTest,
    ::testing::Values(
        // Edge case: n = 0
        CountingBitsTestCase{0, {0}},
        // Simple case: n = 1
        CountingBitsTestCase{1, {0, 1}},
        // Small n: n = 2
        CountingBitsTestCase{2, {0, 1, 1}},
        // Example from problem: n = 5
        CountingBitsTestCase{5, {0, 1, 1, 2, 1, 2}},
        // Power of two: n = 8
        CountingBitsTestCase{8, {0, 1, 1, 2, 1, 2, 2, 3, 1}},
        // Larger n: n = 15
        CountingBitsTestCase{
            15, {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4}}));
