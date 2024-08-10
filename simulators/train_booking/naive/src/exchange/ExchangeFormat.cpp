#include "ExchangeFormat.hpp"

#include <range/v3/all.hpp>

namespace app {

void setClientMetaData(exchange_format::Response &response, UniqUserId uuid) {
  response.mutable_client()->set_uuid(std::move(uuid));
}

exchange_format::Response toResponse(response::Variants &response) {
  // TODO:
}

request::Variants fromRequest(exchange_format::Request &request) {
  using namespace exchange_format;
  switch (request.payload_case()) {
  case Request::kVacantCarriages: {
    return request::VacantCars{};
  }
  case Request::kVacantSeats: {
    auto carId = request.vacant_seats().carriage().carriage_id();
    // While internal train API allows for gathering information from multiple
    // carriages, it is not provided to the outer world
    return request::VacantSeats{{carId}};
  }
  case Request::kBooking: {
    request.booking().seats() |
        ranges::views::transform([](auto &&kv) -> CarAndSeat {
          return CarAndSeat{
              kv.carriage().carriage_id(),
              kv.seat_id(),
          };
        });
  } break;
  case Request::kProfile: {
    return {};
  }
  default:
    break;
  }
  throw std::invalid_argument("request is invalid");
}

UniqUserId getClientMetaData(exchange_format::Request &request) {
  return request.client().uuid();
}
} // namespace app
