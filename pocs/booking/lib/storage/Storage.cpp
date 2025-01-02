#include "Storage.hpp"

#include <fstream>
#include <ranges>

namespace {
std::size_t indexToPlaceNum(std::size_t idx) noexcept { return idx + 1; }
} // namespace

namespace storage {
std::optional<std::size_t> TicketStorage::tryBook(UserId user) noexcept {
  // Предполагаем что потоков много меньше чем количество вакантных мест.
  // Также считаем что после получения nullopt работа сервера завершается.
  auto idx = vacant_.fetch_add(1UL);
  if (idx >= tickets_.size()) {
    return std::nullopt;
  }
  tickets_[idx] = std::move(user);
  return indexToPlaceNum(idx);
}

bool TicketStorage::serialize() {
  // Нужно предотвратить возможность гонок. В данном случае используем простой
  // подход. Сериализация означает конец работы.
  vacant_.store(tickets_.size());

  // Проверяем, существует ли файл, и удаляем его, если необходимо
  if (std::filesystem::exists(filePath_)) {
    std::filesystem::remove(filePath_);
  }

  if (std::ofstream outFile(filePath_); outFile.is_open()) {
    outFile << "Ticket,Client\n";
    for (auto const [idx, uuid] : std::ranges::enumerate_view(tickets_)) {
      outFile << indexToPlaceNum(idx) << ',' << uuid << '\n';
    }
    return true;
  }
  // TODO: логировать
  return false;
}
} // namespace storage