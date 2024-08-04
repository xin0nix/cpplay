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

std::vector<std::size_t> Train::getVacantSeats(std::size_t carriage) {
  return mCarriages.at(carriage) | ranges::views::enumerate |
         ranges::views::filter([](auto const &kv) { return !kv.second; }) |
         ranges::views::transform([](auto const &kv) { return kv.first; }) |
         ranges::to<std::vector>;
}

bool Train::tryToBook(std::size_t carriage, std::size_t seat) {}
} // namespace app
