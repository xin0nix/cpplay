#include <cstdint>
#include <gtest/gtest.h>
#include <iostream>

// Assume Solution class is defined elsewhere

class Solution {
public:
  uint32_t reverseBits(uint32_t value) {
    constexpr uint32_t highestBit = 0x80'00'00'00;
    uint32_t result = 0;
    uint32_t reMask = highestBit;
    uint32_t mask = 1;
    for (int i = 0; i < 32; ++i) {
      if ((value & mask) > 0) {
        result |= reMask;
      }
      mask <<= 1;
      reMask >>= 1;
    }
    return result;
  }
};

struct ReverseBitsTestCase {
  uint32_t input;
  uint32_t expected;
};

class ReverseBitsTest : public ::testing::TestWithParam<ReverseBitsTestCase> {};

TEST_P(ReverseBitsTest, HandlesVariousInputs) {
  Solution sol;
  const auto &param = GetParam();
  EXPECT_EQ(sol.reverseBits(param.input), param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    ReverseBitsTests, ReverseBitsTest,
    ::testing::Values(
        // Example 1: Input: 0b00000010100101000001111010011100, Output:
        // 0b00111001011110000010100101000000
        ReverseBitsTestCase{0b00000010100101000001111010011100u,
                            0b00111001011110000010100101000000u},

        // Example 2: Input: 0b11111111111111111111111111111101, Output:
        // 0b10111111111111111111111111111111
        ReverseBitsTestCase{0b11111111111111111111111111111101u,
                            0b10111111111111111111111111111111u},

        // All bits zero
        ReverseBitsTestCase{0u, 0u},

        // All bits one
        ReverseBitsTestCase{0xFFFFFFFFu, 0xFFFFFFFFu},

        // Only the least significant bit set
        ReverseBitsTestCase{1u, 0x80000000u},

        // Only the most significant bit set
        ReverseBitsTestCase{0x80000000u, 1u},

        // Alternate bits (1010...)
        ReverseBitsTestCase{0xAAAAAAAAu, 0x55555555u},

        // Alternate bits (0101...)
        ReverseBitsTestCase{0x55555555u, 0xAAAAAAAAu}));
