#include <boost/asio/ip/tcp.hpp>
#include <boost/program_options.hpp>

#include "CoroutineServer.hpp"
#include "Server.hpp"
#include "ThreadPoolServer.hpp"
#include "boost/any/bad_any_cast.hpp"

#include <exception>
#include <filesystem>
#include <format>
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

int main(int ac, char *av[]) {
  po::options_description desc("Опции командной строки");
  // clang-format off
  desc.add_options()
    ("help", "Показать справку")
    ("backend", po::value<std::string>()->required(), "Бэкенд: coro или tpool")
    ("port", po::value<std::uint16_t>()->default_value(8080), "Номер порта сервера")
    ("address", po::value<std::string>()->default_value("127.0.0.1"), "IP адрес сервера")
    ("max-backlog", po::value<std::uint16_t>()->default_value(10), "Мкс. количество ожидающих клиентов")
    ("db-path", po::value<std::string>()->default_value("db.csv"), "База данных брони")
    ("capacity", po::value<std::size_t>()->default_value(10'000), "Количество мест")
    ("delay", po::value<std::uint64_t>()->default_value(0UL), "Искусственная задержка на запрос (нс)")
    ("threads", po::value<std::uint16_t>()->default_value(1), "Количество потоков");
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
    uint16_t port{};
    std::string addressArg{};
    uint16_t maxBacklog{};
    fs::path dbPath{};
    std::size_t capacity{};
    std::chrono::milliseconds delay{};
    std::string backend{};
    std::uint16_t numThreads{};
    try {
      port = varMap["port"].as<uint16_t>();
      addressArg = varMap["address"].as<std::string>();
      maxBacklog = varMap["max-backlog"].as<uint16_t>();
      dbPath = varMap["db-path"].as<std::string>();
      capacity = varMap["capacity"].as<std::size_t>();
      delay = std::chrono::milliseconds(varMap["delay"].as<std::uint64_t>());
      backend = varMap["backend"].as<std::string>();
      numThreads = varMap["threads"].as<std::uint16_t>();
    } catch (boost::bad_any_cast &cast) {
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }

    std::cout << "Бэкенд: " << backend << std::endl;
    std::cout << "Порт: " << port << std::endl;
    std::cout << "Адрес: " << addressArg << std::endl;
    std::cout << "Бэклог: " << maxBacklog << std::endl;
    std::cout << "Свободных мест: " << capacity << std::endl;
    std::cout << "Путь к базе данных: " << dbPath << std::endl;
    std::cout << "Искусственная задержка: " << delay << std::endl;
    std::cout << "Количество потоков: " << numThreads << std::endl;

    const std::regex ipRegex(R"(^\d{1,3}(\.\d{1,3}){3}$)");
    if (not std::regex_match(addressArg, ipRegex)) {
      throw std::invalid_argument("Кривой IP, проверь ещё раз");
    }

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
    rng::for_each(bytesRange | view::enumerate, [&bytes](auto &&kv) {
      auto &&[idx, val] = kv;
      bytes.at(idx) = val;
    });
    boost::asio::ip::address_v4 address(bytes);
    auto endpoint = boost::asio::ip::tcp::endpoint(address, port);
    std::unique_ptr<Server> server;
    if (backend == "coro") {
      server = std::make_unique<CoroutineServer>(std::move(endpoint),
                                                 maxBacklog, std::move(dbPath),
                                                 capacity, delay, numThreads);
    } else if (backend == "tpool") {
      server = std::make_unique<ThreadPoolServer>(std::move(endpoint),
                                                  maxBacklog, std::move(dbPath),
                                                  capacity, delay, numThreads);
    } else {
      throw std::invalid_argument(backend);
    }
    server->serve();
  } catch (std::exception &e) {
    std::cerr << std::format("Проблема: {}\n", e.what());
    return EXIT_FAILURE;
  }

  return 0;
}
