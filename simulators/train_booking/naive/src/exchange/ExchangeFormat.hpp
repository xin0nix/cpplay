#include "ExchangeFormat.pb.h"
#include "Train.hpp"
#include <variant>

namespace app {

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
using Variants = std::variant<Error, VacantCarriages, VacantCarriages>;
} // namespace response
exchange_format::Response toResponse(response::Variants &&errorMessage);

namespace request {
struct VacantCars {};
struct VacantSeats {
  std::vector<Car> cars;
};
struct TryToBook {
  std::vector<CarAndSeat> seats;
};
using Variants = std::variant<VacantCars, VacantSeats, TryToBook>;
} // namespace request
request::Variants fromRequest(exchange_format::Request &&request);

} // namespace app
