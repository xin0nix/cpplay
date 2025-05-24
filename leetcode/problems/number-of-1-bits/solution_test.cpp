#include <gtest/gtest.h>
#include <limits>

// Assume Solution class is defined elsewhere
class Solution {
public:
  int hammingWeight(uint32_t value) {
    int counter = 0;
    for (uint64_t mask = 1; mask <= std::numeric_limits<uint32_t>::max();
         mask <<= 1) {
      if ((static_cast<uint32_t>(mask) & value) > 0U) {
        counter++;
      }
    }
    return counter;
  }
};

struct HammingWeightTestCase {
  uint32_t input;
  int expected;
};

class HammingWeightTest
    : public ::testing::TestWithParam<HammingWeightTestCase> {};

TEST_P(HammingWeightTest, HandlesVariousInputs) {
  Solution sol;
  const auto &param = GetParam();
  EXPECT_EQ(sol.hammingWeight(param.input), param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    HammingWeightTests, HammingWeightTest,
    ::testing::Values(HammingWeightTestCase{0b00000000000000000000000000001011,
                                            3}, // Example 1
                      HammingWeightTestCase{0b00000000000000000000000010000000,
                                            1}, // Example 2
                      HammingWeightTestCase{0b11111111111111111111111111111101,
                                            31},             // Example 3
                      HammingWeightTestCase{0, 0},           // All bits zero
                      HammingWeightTestCase{0xFFFFFFFF, 32}, // All bits one
                      HammingWeightTestCase{0x80000000, 1},  // Only MSB set
                      HammingWeightTestCase{1, 1},           // Only LSB set
                      HammingWeightTestCase{0xAAAAAAAA,
                                            16}, // Alternate bits set (1010...)
                      HammingWeightTestCase{0x55555555, 16}
                      // Alternate bits set (0101...)
                      ));
