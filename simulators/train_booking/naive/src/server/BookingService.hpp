#pragma once

#include <string>
#include <unordered_map>

#include "ExchangeFormat.hpp"
#include "Train.hpp"

namespace app {
struct BookingService {
  response::Variants handle(const UUIDType &userId, request::Profile &);
  response::Variants handle(const UUIDType &userId, request::VacantCars &cars);
  response::Variants handle(const UUIDType &userId,
                            request::VacantSeats &seats);
  response::Variants handle(const UUIDType &userId,
                            request::TryToBook &booking);

private:
  struct UserProfile {
    std::string mFirstName;
    std::string mLastName;
  };

  Train mTrain;
  std::unordered_map<UUIDType /*uuid*/, UserProfile> mUsers;
};
} // namespace app
