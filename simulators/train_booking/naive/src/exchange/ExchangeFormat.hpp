#pragma once

#include "ExchangeFormat.pb.h"
#include "Train.hpp"
#include <variant>

namespace app {

// helper type for the visitor
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

using UniqUserId = std::string;
using CorrelationId = std::string;

namespace response {
struct Error {
  std::string message;
};
struct VacantCarriages {
  std::vector<Car> cars;
};
struct VacantSeats {
  std::vector<CarAndSeat> seats;
};
struct BookingAttempt {
  bool success;
};
struct Profile {
  std::string firstName;
  std::string lastName;
};
using Variants =
    std::variant<Error, VacantCarriages, VacantSeats, Profile, BookingAttempt>;
} // namespace response
exchange_format::Response toResponse(response::Variants &&response);
void setClientMetaData(exchange_format::Response &response,
                       const std::pair<UniqUserId, CorrelationId> &profile);
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
std::pair<UniqUserId, CorrelationId>
getClientMetaData(exchange_format::Request &request);

} // namespace app
