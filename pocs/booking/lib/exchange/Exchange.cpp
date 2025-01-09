#include "Exchange.hpp"

#include <ranges>

namespace rng = std::ranges;
namespace view = std::views;

namespace exchange {
std::expected<uint64_t, DecodeVarIntError>
decodeVarint(std::span<uint8_t> data) {
  // Пустого массива данных быть не должно
  if (data.empty()) {
    return std::unexpected(DecodeVarIntError::kInvalidInput);
  }
  // Ноль это валидное целое число (см исключение ниже)
  if (data.size() == 1UL and data.front() == 0UL) {
    return 0UL;
  }
  // Чекнем 4-й байт, у него MSB болжен быть 0
  if (data.size() >= 4UL and data[3UL] & 0x80) {
    return std::unexpected(DecodeVarIntError::kOverflow);
  }
  uint64_t result = 0UL;
  // Берём не больше первых 4-х байт, причём так, чтобы у последнего MSB был
  // 0-ым
  bool proceed = true;
  auto bytes =
      data | view::take(4) | view::take_while([&proceed](auto byte) -> bool {
        if (!proceed) {
          return false;
        }
        proceed = byte & 0x80;
        return true;
      });
  result = rng::fold_left(bytes | view::enumerate, 0UL, [](auto accum, auto b) {
    auto &&[idx, byte] = b;
    auto shift = idx * 7;
    return accum | (byte & 0x7F) << shift;
  });
  // Больше одного байта на входе но мы получили ноль (все байты 0x80):
  if (result == 0) {
    return std::unexpected(DecodeVarIntError::kInvalidInput);
  }
  return result;
}
} // namespace exchange