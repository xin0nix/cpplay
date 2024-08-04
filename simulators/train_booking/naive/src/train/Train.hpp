#include <array>
#include <vector>

namespace app {
constexpr std::size_t kNumCarriages = 10;
constexpr std::size_t kNumSeats = 60;
struct Train final {
  std::vector<std::size_t> getVacantCarriages();
  std::vector<std::size_t> getVacantSeats(std::size_t carriageNum);
  bool tryToBook(std::size_t carriageNum, std::size_t seatNum);
  std::array<std::array<bool, kNumSeats>, kNumCarriages> mCarriages;
};
} // namespace app
