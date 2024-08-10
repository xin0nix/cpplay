#include "ExchangeFormat.hpp"
#include "Train.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <range/v3/all.hpp>

TEST(ExchangeFormatTest, RequestVacantSeats) {
  exchange_format::Request request;
  request.mutable_vacant_seats()->mutable_carriage()->set_carriage_id(3);
  auto var = app::fromRequest(request);
}
