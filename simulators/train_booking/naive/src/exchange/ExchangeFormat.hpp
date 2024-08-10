#pragma once

#include "ExchangeFormat.pb.h"
#include "Train.hpp"
#include <variant>

namespace app {

using UniqUserId = std::string;

namespace response {
struct Error {
  std::string message;
};
struct VacantCarriages {
  std::vector<Car> vacant_carriages;
};
struct VacantSeats {
  std::vector<CarAndSeat> seats;
};
struct Profile {
  std::string FirstName;
  std::string LastName;
};
using Variants = std::variant<Error, VacantCarriages, VacantCarriages, Profile>;
} // namespace response
exchange_format::Response toResponse(response::Variants &&response);
void setClientMetaData(exchange_format::Response &response, UniqUserId uuid);

namespace request {
struct VacantCars {};
struct VacantSeats {
  std::vector<Car> cars;
};
struct TryToBook {
  std::vector<CarAndSeat> seats;
};
struct Profile {};
using Variants = std::variant<VacantCars, VacantSeats, TryToBook, Profile>;
} // namespace request
request::Variants fromRequest(exchange_format::Request &request);
UniqUserId getClientMetaData(exchange_format::Request &request);

} // namespace app
