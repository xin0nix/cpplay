#include "Train.hpp"
#include <algorithm>
#include <ranges>

namespace app {

namespace rs = std::ranges;
namespace rv = rs::views;

std::vector<Car> Train::getVacantCarriages() {
  std::vector<Car> vacantCars;
  for (auto [carId, car] : rv::enumerate(mCarriages)) {
    if (rs::any_of(car,
                   [](auto isVacantSeat) { return isVacantSeat == false; })) {
      vacantCars.push_back(carId);
    }
  }
  return vacantCars;
}

std::vector<Seat> Train::getVacantSeats(Car carriageNum) {
  std::vector<Seat> vacantSeats;
  for (auto [seatId, isVacant] : rv::enumerate(mCarriages.at(carriageNum))) {
    if (isVacant == false) {
      vacantSeats.push_back(seatId);
    }
  }
  return vacantSeats;
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
