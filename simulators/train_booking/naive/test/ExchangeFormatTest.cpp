#include "ExchangeFormat.hpp"
#include "Train.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <range/v3/all.hpp>

using ::testing::ElementsAre;

TEST(ExchangeFormatTest, RequestProfile) {
  exchange_format::Request request;
  request.set_allocated_profile(new exchange_format::ProfileRequest{});
  auto var = app::fromRequest(request);
  ASSERT_TRUE(std::holds_alternative<app::request::Profile>(var));
}

TEST(ExchangeFormatTest, RequestVacantCarriages) {
  exchange_format::Request request;
  request.set_allocated_vacant_carriages(
      new exchange_format::VacantCarriagesRequest{});
  auto var = app::fromRequest(request);
  ASSERT_TRUE(std::holds_alternative<app::request::VacantCars>(var));
}

TEST(ExchangeFormatTest, RequestVacantSeats) {
  exchange_format::Request request;
  request.mutable_vacant_seats()->mutable_carriage()->set_carriage_id(3);
  auto var = app::fromRequest(request);
  ASSERT_TRUE(std::holds_alternative<app::request::VacantSeats>(var));
  auto seats = std::get<app::request::VacantSeats>(var);
  ASSERT_THAT(seats.cars, ElementsAre(app::Car{3}));
}

TEST(ExchangeFormatTest, BookingRequest) {
  exchange_format::Request request;
  auto makeCarAndSeat = [](size_t carId, size_t seatId) {
    auto *car = new exchange_format::Carriage;
    car->set_carriage_id(carId);
    exchange_format::Seat seat;
    seat.set_allocated_carriage(car);
    seat.set_seat_id(seatId);
    return seat;
  };
  auto *booking = new exchange_format::BookingRequest{};
  request.set_allocated_booking(booking);
  request.mutable_booking()->mutable_seats()->Add(makeCarAndSeat(1, 57));
  request.mutable_booking()->mutable_seats()->Add(makeCarAndSeat(2, 13));
  request.mutable_booking()->mutable_seats()->Add(makeCarAndSeat(2, 29));
  auto var = app::fromRequest(request);
  ASSERT_TRUE(std::holds_alternative<app::request::TryToBook>(var));
  auto tryToBook = std::get<app::request::TryToBook>(var);
  ASSERT_THAT(tryToBook.seats,
              ElementsAre(app::CarAndSeat{1, 57}, app::CarAndSeat{2, 13},
                          app::CarAndSeat{2, 29}));
}
