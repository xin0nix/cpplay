#include "Exchange.hpp"
#include "Exchange.pb.h"
#include "Storage.hpp"
#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <exception>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <regex>
#include <stdexcept>
#include <string>

namespace po = boost::program_options;
namespace fs = std::filesystem;
namespace rng = std::ranges;
namespace view = std::views;
using namespace std::chrono_literals;

using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::ip::tcp;

struct Server {
  // Корутина для обработки клиентского подключения
  awaitable<void> handle_client(tcp::socket socket) {
    try {
      std::array<uint8_t, 1024> messageBuffer;
      std::string message;
      for (;;) {
        // Чтение данных из сокета
        std::size_t bytesRead = co_await socket.async_read_some(
            boost::asio::buffer(messageBuffer), use_awaitable);
        // Узнаем размер сообщения, потом если что дочитаем оставшиеся байты
        std::span<uint8_t> data(messageBuffer.data(), bytesRead);
        auto messageLength = exchange::decodeVarint(data);
        if (!messageLength) {
          std::printf("Не получилось декодировать размер сообщения, считано "
                      "байт: %zu\n",
                      bytesRead);
          co_return;
        }
        rng::copy(data, std::back_inserter(message));
        while (bytesRead < *messageLength) {
          // Переиспользуем буффер, выше мы уже скопировали информацию в
          // сообщение
          bytesRead = co_await socket.async_read_some(
              boost::asio::buffer(messageBuffer), use_awaitable);
          rng::copy(data, std::back_inserter(message));
        }

        // Сервер работает из предположения что клиент не станет пихать
        // несколько сообщение подряд. В противном случае передача
        // рассинхронизируется.
        if (message.size() != bytesRead) {
          std::printf(
              "Количество байтов не совпало с размером proto сообщения: "
              "%zu != %zu\n",
              bytesRead, message.size());
          co_return;
        }

        exchange::Request request{};
        if (!request.ParseFromString(message)) {
          std::printf("Пришло битое proto сообщение");
          co_return;
        }

        // TODO: попытка брони

        // TODO: delay

        // TODO: Запись данных обратно в сокет
        // co_await async_write(socket, boost::asio::buffer(data, n),
        // use_awaitable);
      }
    } catch (std::exception &e) {
      std::printf("Произошла ошибка при работе с клиентским подключением: %s\n",
                  e.what());
    }
  }

  // Корутина основного цикла сервера
  awaitable<void> start(tcp::acceptor &acceptor) {
    for (;;) {
      // Сервер в цикле опрашивает сокет на предмет новых клиентов
      tcp::socket socket = co_await acceptor.async_accept(use_awaitable);

      // Мы оборачиваем каждое клиентское подключение в strand чтобы избежать
      // состояния гонок между его корутинами без доп синхронизации
      auto client_strand = boost::asio::make_strand(acceptor.get_executor());

      // Запуск корутины для обработки клиента в контексте strand
      co_spawn(client_strand, handle_client(std::move(socket)), detached);

      // TODO: сохранение "базы данных"
    }
  }

  Server(fs::path dbPath, std::size_t capacity, std::chrono::milliseconds delay)
      : storage_(std::move(dbPath), capacity), delay_(delay) {}

  storage::TicketStorage storage_;
  std::chrono::milliseconds delay_;
};

int main(int ac, char *av[]) {
  po::options_description desc("Опции командной строки");
  // clang-format off
  desc.add_options()
    ("help", "Показать справку")
    ("port", po::value<std::uint16_t>()->default_value(8080), "Номер порта сервера")
    ("address", po::value<std::string>()->default_value("127.0.0.1"), "IP адрес сервера")
    ("max-backlog", po::value<std::uint16_t>()->default_value(10), "Мкс. количество ожидающих клиентов")
    ("db-path", po::value<std::string>()->default_value("db.csv"), "База данных брони")
    ("capacity", po::value<std::size_t>()->default_value(10'000), "Количество мест")
    ("delay", po::value<std::uint64_t>()->default_value(0UL), "Искусственная задержка на запрос (нс)")
    ;
  // clang-format on
  try {
    po::variables_map varMap;
    po::store(po::parse_command_line(ac, av, desc), varMap);
    po::notify(varMap);

    if (varMap.count("help")) {
      // TODO: replace cout with logger
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }

    // TODO: отлов исключения boost::bad_any_cast
    auto port = varMap["port"].as<uint16_t>();
    auto addressArg = varMap["address"].as<std::string>();
    auto maxBacklog = varMap["max-backlog"].as<uint16_t>();
    fs::path dbPath = varMap["db-path"].as<std::string>();
    auto capacity = varMap["capacity"].as<std::size_t>();
    auto delay = std::chrono::milliseconds(varMap["delay"].as<std::uint64_t>());

    std::cout << "Порт: " << port << std::endl;
    std::cout << "Адрес: " << addressArg << std::endl;
    std::cout << "Бэклог: " << maxBacklog << std::endl;
    std::cout << "Свободных мест: " << capacity << std::endl;
    std::cout << "Путь к базе данных: " << dbPath << std::endl;

    const std::regex ipRegex(R"(^\d{1,3}(\.\d{1,3}){3}$)");
    if (not std::regex_match(addressArg, ipRegex)) {
      throw std::invalid_argument("Кривой IP, проверь ещё раз");
    }

    Server server(dbPath, capacity, delay);
    boost::asio::io_context io_context(4);
    // TODO: проверить addressArg через regex
    using std::operator""sv;
    auto bytesRange =
        view::split(addressArg, "."sv) |
        view::transform([](auto &&octetRange) -> std::uint8_t {
          auto val = boost::lexical_cast<std::uint32_t>(
              std::string(octetRange.cbegin(), octetRange.cend()));
          if (val > 255) {
            throw std::invalid_argument(
                "Числа внутри IP должны быть от 0 до 255 (включительно)");
          }
          return val;
        });
    std::array<uint8_t, 4> bytes{0};
    rng::for_each(bytesRange | view::take(4) | view::enumerate,
                  [&bytes](auto &&kv) {
                    auto &&[idx, val] = kv;
                    bytes.at(idx) = val;
                  });
    boost::asio::ip::address_v4 address(bytes);

    // Создание акцептора для прослушивания порта
    auto endpoint = tcp::endpoint(address, port);
    tcp::acceptor acceptor(io_context, endpoint);

    // Запуск сервера
    co_spawn(io_context, server.start(acceptor), detached);

    // Запуск io_context
    io_context.run();
  } catch (std::exception &e) {
    std::printf("Проблема: %s\n", e.what());
  }

  return 0;
}
