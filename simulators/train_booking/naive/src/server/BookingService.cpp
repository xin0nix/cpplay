#include "BookingService.hpp"

namespace app {
response::Variants BookingService::handle(const UUIDType &userId,
                                          request::Profile &msg) {
  auto userIt = mUsers.find(userId);
  if (userIt != mUsers.cend()) {
    return response::Error{.message = "User already registered"};
  }
  mUsers.insert({userId, UserProfile{.mFirstName = msg.firstName,
                                     .mLastName = msg.lastName}});
  return response::Profile{
      .success = true,
  };
}

response::Variants BookingService::handle(const UUIDType &userId,
                                          request::VacantCars &cars) {}

response::Variants BookingService::handle(const UUIDType &userId,
                                          request::VacantSeats &seats) {}

response::Variants BookingService::handle(const UUIDType &userId,
                                          request::TryToBook &booking) {}

} // namespace app
