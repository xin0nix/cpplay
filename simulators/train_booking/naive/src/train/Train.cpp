#include "Train.hpp"
#include <range/v3/all.hpp>

namespace app {
std::vector<std::size_t> Train::getVacantCarriages() {
  std::vector<std::size_t> result;
  result.reserve(kNumCarriages); // kNumCarriages == 10
  for (auto const &[index, carriage] : ranges::views::enumerate(mCarriages)) {
    if (ranges::any_of(carriage, [](bool isBooked) { return !isBooked; })) {
      result.push_back(index);
    }
  }
  return std::move(result);
}

std::vector<std::size_t> Train::getVacantSeats(std::size_t carriage) {}

bool Train::tryToBook(std::size_t carriage, std::size_t seat) {}
} // namespace app
