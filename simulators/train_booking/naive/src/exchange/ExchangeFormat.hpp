#include "ExchangeFormat.pb.h"
#include "Train.hpp"
#include <memory>
#include <span>

namespace app {
using MessagePtr = std::unique_ptr<google::protobuf::Message>;
MessagePtr toVacantCarsResponse(std::span<Car> cars);
MessagePtr toVacantSeatsResponse(std::span<CarAndSeat> seats);
} // namespace app
