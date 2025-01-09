#include "Exchange.hpp"

#include <gtest/gtest.h>

using namespace exchange;

TEST(DecodeVarIntTest, EmptyInput) {
  std::span<uint8_t> emptyData;
  auto result = decodeVarint(emptyData);
  EXPECT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DecodeVarIntError::kInvalidInput);
}

TEST(DecodeVarIntTest, SingleByteInput) {
  uint8_t data[] = {0x7F};
  auto result = decodeVarint(data);
  ASSERT_TRUE(result.has_value());
  // Все единицы кроме MSB
  EXPECT_EQ(result.value(), 0x7F);
}

TEST(DecodeVarIntTest, TwoByteInput) {
  uint8_t data[] = {0x81, 0x01};
  auto result = decodeVarint(data);
  EXPECT_TRUE(result.has_value());
  // первый бит 1-го байта становится последним битом 0-го, получаем 0x81
  EXPECT_EQ(result.value(), 0x81);
}

TEST(DecodeVarIntTest, ThreeByteInput) {
  uint8_t data[] = {0x81, 0x82, 0x03};
  auto result = decodeVarint(data);
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), 0x01 | (0x02 << 7) | (0x03 << 14));
}

TEST(DecodeVarIntTest, FourByteInput) {
  uint8_t data[] = {0x81, 0x82, 0x83, 0x04};
  auto result = decodeVarint(data);
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), 0x01 | (0x02 << 7) | (0x03 << 14) | (0x04 << 21));
}

TEST(DecodeVarIntTest, OverflowInput) {
  uint8_t data[] = {0x80, 0x80, 0x80, 0x80};
  auto result = decodeVarint(data);
  EXPECT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DecodeVarIntError::kOverflow);
}

TEST(DecodeVarIntTest, InvalidInput) {
  uint8_t data[] = {0x80, 0x80, 0x80, 0x00};
  auto result = decodeVarint(data);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DecodeVarIntError::kInvalidInput);
}
