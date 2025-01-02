#pragma once

#include <atomic>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace storage {
using UserId = std::string;
namespace fs = std::filesystem;

struct TicketStorage {
  explicit TicketStorage(fs::path filePath, std::size_t capacity) noexcept
      : filePath_(std::move(filePath)), tickets_(capacity) {}

  std::optional<std::size_t> tryBook(UserId user) noexcept;

  bool serialize();

  bool hasVacantSeats() const noexcept { return vacant_ < tickets_.size(); }

private:
  fs::path filePath_;
  std::atomic<std::size_t> vacant_{0UL};
  std::vector<UserId> tickets_;
};

} // namespace storage