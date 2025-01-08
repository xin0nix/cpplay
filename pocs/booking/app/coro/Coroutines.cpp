#include "Exchange.hpp"
#include "Storage.hpp"
#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <sstream>
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

// Корутина для обработки клиентского подключения
awaitable<void> handle_client(tcp::socket socket) {
  try {
    char data[1024];
    for (;;) {
      // Чтение данных из сокета
      std::size_t n = co_await socket.async_read_some(boost::asio::buffer(data),
                                                      use_awaitable);
      // Запись данных обратно в сокет
      co_await async_write(socket, boost::asio::buffer(data, n), use_awaitable);
    }
  } catch (std::exception &e) {
    std::printf("Произошла ошибка при работе с клиентским подключением: %s\n",
                e.what());
  }
}

// Функция для запуска сервера
awaitable<void> start_server(tcp::acceptor &acceptor) {
  for (;;) {
    // Принятие нового клиентского подключения
    tcp::socket socket = co_await acceptor.async_accept(use_awaitable);

    // Создание strand для этого клиента
    auto client_strand = boost::asio::make_strand(acceptor.get_executor());

    // Запуск корутины для обработки клиента в контексте strand
    co_spawn(client_strand, handle_client(std::move(socket)), detached);
  }
}

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
    // auto delay =
    // std::chrono::milliseconds(varMap["delay"].as<std::uint64_t>());

    std::cout << "Порт: " << port << std::endl;
    std::cout << "Адрес: " << addressArg << std::endl;
    std::cout << "Бэклог: " << maxBacklog << std::endl;
    std::cout << "Свободных мест: " << capacity << std::endl;
    std::cout << "Путь к базе данных: " << dbPath << std::endl;

    storage::TicketStorage storage(dbPath, capacity);
    boost::asio::io_context io_context(4);

    auto octets =
        rng::to<std::vector<std::string>>(addressArg | view::split('.'));
    auto bytesRange =
        octets | view::transform([](auto &&octet) -> std::uint8_t {
          std::uint8_t val = 0;
          std::istringstream iss(octet);
          iss >> val;
          return val;
        });
    if (octets.size() != 4) {
      throw std::invalid_argument("bad ip address");
    }
    std::array<uint8_t, 4> bytes;
    rng::for_each(bytesRange | view::enumerate, [&bytes](auto &&kv) {
      auto &&[idx, val] = kv;
      bytes.at(idx) = val;
    });
    boost::asio::ip::address_v4 address(bytes);

    // Создание акцептора для прослушивания порта
    auto endpoint = tcp::endpoint(address, port);
    tcp::acceptor acceptor(io_context, endpoint);

    // Запуск сервера
    co_spawn(io_context, start_server(acceptor), detached);

    // Запуск io_context
    io_context.run();
  } catch (std::exception &e) {
    std::printf("Произошла ошибка: %s\n", e.what());
  }

  return 0;
}
