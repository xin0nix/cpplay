#include "BookingService.hpp"
#include <range/v3/all.hpp>

#include <iostream>

namespace app {
response::Variants BookingService::handle(const UUIDType &userId,
                                          request::Profile &msg) {
  try {
    auto userIt = mUsers.find(userId);
    if (userIt != mUsers.cend()) {
      return response::Error{.message = "User already registered"};
    }
    std::cout << "Registering new user with UUID " << userId
              << ", name: " << msg.firstName << ", last name: " << msg.lastName
              << std::endl;
    mUsers.insert({userId, UserProfile{.mFirstName = msg.firstName,
                                       .mLastName = msg.lastName}});
    return response::Profile{
        .success = true,
    };
  } catch (...) {
    return response::Error("Internal error");
  }
}

response::Variants BookingService::handle(const UUIDType &userId,
                                          request::VacantCars &cars) {
  try {
    std::ignore = userId;
    auto vacantCars = mTrain.getVacantCarriages();
    return response::VacantCarriages{
        .cars = vacantCars,
    };
  } catch (...) {
    return response::Error("Internal error");
  }
}

response::Variants BookingService::handle(const UUIDType &userId,
                                          request::VacantSeats &seats) {
  try {
    std::ignore = userId;
    response::VacantSeats res;
    for (auto car : seats.cars) {
      auto vacantSeats = mTrain.getVacantSeats(car);
      for (auto seat : vacantSeats) {
        res.seats.emplace_back(car, seat);
      }
    }
    return std::move(res);
  } catch (...) {
    return response::Error("Internal error");
  }
}

response::Variants BookingService::handle(const UUIDType &userId,
                                          request::TryToBook &booking) {
  try {
    // FIXME: log out correlation id as well
    std::cout << "User with UUID " << userId << " tried to book "
              << booking.seats.size() << " seats" << std::endl;
    bool status = mTrain.tryToBook(booking.seats);
    return response::BookingAttempt{
        .success = status,
    };
  } catch (...) {
    return response::Error("Internal error");
  }
}

} // namespace app
