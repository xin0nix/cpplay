#include "ExchangeFormat.hpp"

#include <range/v3/all.hpp>

namespace app {

void setClientMetaData(exchange_format::Response &response, UniqUserId uuid,
                       CorrelationId corId) {
  response.mutable_client()->set_uuid(std::move(uuid));
  // TODO: set correlation id
}

exchange_format::Response toResponse(response::Variants &response) {
  return std::visit(
      overloaded{
          [](response::Error err) {
            exchange_format::Response rp;
            auto *errorRp = new exchange_format::ErrorResponse{};
            errorRp->set_message(err.message);
            rp.set_allocated_error(errorRp);
            return rp;
          },
          [](response::VacantCarriages vacantCars) {
            exchange_format::Response rp;
            auto *carsRp = new exchange_format::VacantCarriagesResponse{};
            auto *cars = carsRp->mutable_carriages();
            for (auto vCar : vacantCars.cars) {
              cars->Add([&vCar] {
                exchange_format::Carriage car;
                car.set_carriage_id(vCar);
                return car;
              }());
            }
            rp.set_allocated_vacant_carriages(carsRp);
            return rp;
          },
          [](response::VacantSeats vacantSeats) {
            exchange_format::Response rp;
            auto *seatsRp = new exchange_format::VacantSeatsResponse{};
            auto *seats = seatsRp->mutable_seats();
            for (auto carAndSeat : vacantSeats.seats) {
              exchange_format::Seat seat;
              auto *car = new exchange_format::Carriage{};
              car->set_carriage_id(carAndSeat.first);
              seat.set_allocated_carriage(car);
              seat.set_seat_id(carAndSeat.second);
              seats->Add(std::move(seat));
            }
            rp.set_allocated_vacant_seats(seatsRp);
            return rp;
          },
          [](response::Profile profile) {
            exchange_format::Response rp;
            auto profileRp = new exchange_format::ProfileResponse{};
            profileRp->set_firstname(profile.firstName);
            profileRp->set_lastname(profile.lastName);
            rp.set_allocated_profile(profileRp);
            return rp;
          },
          [](response::BookingAttempt bookingAttempt) {
            exchange_format::Response rp;
            auto *bookingRp = new exchange_format::BookingResponse{};
            rp.set_allocated_booking(bookingRp);
            return rp;
          },
      },
      response);
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
    return request::VacantSeats{.cars = {carId}};
  }
  case Request::kBooking: {
    return request::TryToBook{
        .seats = request.booking().seats() |
                 ranges::views::transform([](auto &&kv) -> CarAndSeat {
                   return CarAndSeat{
                       kv.carriage().carriage_id(),
                       kv.seat_id(),
                   };
                 }) |
                 ranges::to<std::vector>};
  }
  case Request::kProfile: {
    return request::Profile{};
  }
  default:
    break;
  }
  throw std::invalid_argument("request is invalid");
}

std::pair<UniqUserId, CorrelationId>
getClientMetaData(exchange_format::Request &request) {
  // TODO: correlation id
  return {request.client().uuid(), ""};
}
} // namespace app
