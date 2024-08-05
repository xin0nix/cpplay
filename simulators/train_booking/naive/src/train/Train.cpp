#include "Train.hpp"
#include <range/v3/all.hpp>

namespace app {
std::vector<Car> Train::getVacantCarriages() {
  return mCarriages | ranges::views::transform([](auto const &car) {
           return ranges::any_of(car, [](bool v) { return !v; });
         }) |
         ranges::views::enumerate |
         ranges::views::filter([](auto const &kv) { return kv.second; }) |
         ranges::views::transform([](auto const &kv) { return kv.first; }) |
         ranges::to<std::vector>;
}

std::vector<Seat> Train::getVacantSeats(Car carriageNum) {
  return mCarriages.at(carriageNum) | ranges::views::enumerate |
         ranges::views::filter([](auto const &kv) { return !kv.second; }) |
         ranges::views::transform([](auto const &kv) { return kv.first; }) |
         ranges::to<std::vector>;
}

bool Train::tryToBook(std::vector<CarAndSeat> &&seats) {
  return tryToBook(std::span(seats));
}

bool Train::tryToBook(std::span<CarAndSeat> seats) {
  if (seats.empty()) {
    throw std::invalid_argument("Empty number of seats cannot be booked");
  }
  for (auto [carNum, seatNum] : seats) {
    auto seat = mCarriages.at(carNum).at(seatNum);
    if (seat) // already booked
      return false;
  }
  for (auto [carNum, seatNum] : seats) {
    mCarriages.at(carNum).at(seatNum) = true;
  }
  return true;
}
} // namespace app
