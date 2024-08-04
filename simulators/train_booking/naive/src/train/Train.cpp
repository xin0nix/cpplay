#include "Train.hpp"
#include <range/v3/all.hpp>

namespace app {
std::vector<std::size_t> Train::getVacantCarriages() {
  std::vector<std::size_t> result;
  result.reserve(kNumCarriages);
  for (auto const &[index, car] : mCarriages | ranges::views::enumerate) {
    if (ranges::any_of(car, [](bool v) { return !v; })) {
      result.push_back(index);
    }
  }
  return std::move(result);
}

std::vector<std::size_t> Train::getVacantSeats(std::size_t carriage) {
  return mCarriages.at(carriage) | ranges::views::enumerate |
         ranges::views::filter([](auto const &kv) { return !kv.second; }) |
         ranges::views::transform([](auto const &kv) { return kv.first; }) |
         ranges::views::take(10) | ranges::to<std::vector>;
}

bool Train::tryToBook(std::size_t carriage, std::size_t seat) {}
} // namespace app
