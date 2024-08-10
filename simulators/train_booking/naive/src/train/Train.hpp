#pragma once

#include <array>
#include <span>
#include <vector>

namespace app {
constexpr std::size_t kNumCarriages = 10;
constexpr std::size_t kNumSeats = 60;

using Car = std::size_t;
using Seat = std::size_t;
using CarAndSeat = std::pair<Car, Seat>;

struct Train final {
  std::vector<Car> getVacantCarriages();
  std::vector<Seat> getVacantSeats(Car carriageNum);
  bool tryToBook(std::vector<CarAndSeat> &&seats);
  bool tryToBook(std::span<CarAndSeat> seats);

  std::array<std::array<bool, kNumSeats>, kNumCarriages> mCarriages;
};
} // namespace app
