#include "Train.hpp"
#include <range/v3/all.hpp>

namespace app {
std::vector<std::size_t> Train::getVacantCarriages() {
  return mCarriages | ranges::views::transform([](auto const &car) {
           return ranges::any_of(car, [](bool v) { return !v; });
         }) |
         ranges::views::enumerate |
         ranges::views::filter([](auto const &kv) { return kv.second; }) |
         ranges::views::transform([](auto const &kv) { return kv.first; }) |
         ranges::to<std::vector>;
}

std::vector<std::size_t> Train::getVacantSeats(std::size_t carriageNum) {
  return mCarriages.at(carriageNum) | ranges::views::enumerate |
         ranges::views::filter([](auto const &kv) { return !kv.second; }) |
         ranges::views::transform([](auto const &kv) { return kv.first; }) |
         ranges::to<std::vector>;
}

bool Train::tryToBook(std::size_t carriageNum, std::size_t seatNum) {
  auto &seat = mCarriages.at(carriageNum).at(seatNum);
  if (seat) // already booked
    return false;
  seat = true;
  return true;
}
} // namespace app
