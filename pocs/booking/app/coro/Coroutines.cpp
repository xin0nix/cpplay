#include "Exchange.hpp"
#include "Exchange.pb.h"
#include "Storage.hpp"
#include "boost/asio/steady_timer.hpp"
#include "boost/asio/this_coro.hpp"

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include <exception>
#include <filesystem>
#include <format>
#include <iostream>
#include <ranges>
#include <regex>
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

struct Server {
  // Корутина для обработки клиентского подключения
  awaitable<void> handle_client(tcp::socket socket) {

    std::array<uint8_t, 1024> messageBuffer;
    std::string message;
    bool done = false;
    while (not done) {
      // Чтение данных из сокета
      std::size_t bytesRead = co_await socket.async_read_some(
          boost::asio::buffer(messageBuffer), use_awaitable);
      // Узнаем размер сообщения, потом если что дочитаем оставшиеся байты
      std::span<uint8_t> data(messageBuffer.data(), bytesRead);
      auto messageLength = exchange::decodeVarint(data);
      if (!messageLength) {
        std::cerr << std::format(
            "Не получилось декодировать размер сообщения, считано "
            "байт: {}\n",
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
        std::cerr << std::format(
            "Количество байтов не совпало с размером proto сообщения: "
            "{} != {}\n",
            bytesRead, message.size());
        co_return;
      }

      exchange::Request request{};
      if (!request.ParseFromString(message)) {
        std::cerr << std::format("Пришло битое proto сообщение длины {}\n",
                                 message.size());
        co_return;
      }
      std::cerr << std::format("Получено новое сообщение: {}\n",
                               request.DebugString());
      auto &&uuid = request.uuid();
      exchange::Response resp;
      resp.set_uuid(uuid);
      if (auto ticket = storage_.tryBook(uuid); ticket.has_value()) {
        auto ok = new exchange::Success{};
        ok->set_ticket_num(ticket.value());
        resp.set_allocated_ok(ok);
        if (delay_.count()) {
          auto executor = co_await boost::asio::this_coro::executor;
          boost::asio::steady_timer timer(executor);
          timer.expires_after(delay_);
          co_await timer.async_wait(use_awaitable);
        }
      } else {
        resp.set_allocated_done(new exchange::Done{});
        done = true;
      }
      std::cout << "Новый ответ: " << resp.DebugString() << '\n';
      std::string msg = resp.SerializeAsString();
      if (msg.empty()) {
        exchange::Error error;
        error.set_what("Ошибка сериализации");
        msg = error.SerializeAsString();
      }
      // TODO: запись в цикле
      co_await async_write(socket, boost::asio::buffer(msg), use_awaitable);
    }
    // Это нормально? Успеют ли остальные корутины отработать??
    ioContext_.stop();
  }

  // Корутина основного цикла сервера
  awaitable<void> listen() {
    // Код ниже выглядит немного странным (ждёт пустой объект??)
    // Но на самом деле это пример механизма tag dispatch
    // Когда помпилятор натыкается на co_await плюс "простой тип"
    // Он ищет подходящий await_transform в текущем контексте
    // И находит перегрузку `await_transform(this_coro::executor_t)`
    auto executor = co_await boost::asio::this_coro::executor;
    tcp::acceptor acceptor(executor, endPoint_);
    for (;;) {
      try {
        // Сервер в цикле опрашивает сокет на предмет новых клиентов
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);

        auto client = socket.remote_endpoint();
        std::cout << std::format("Подключился клиент, IP: {}, Порт: {}\n",
                                 client.address().to_string(), client.port());

        // Мы оборачиваем каждое клиентское подключение в strand чтобы
        // избежать состояния гонок между его корутинами без доп синхронизации
        auto client_strand = boost::asio::make_strand(acceptor.get_executor());

        // Запуск корутины для обработки клиента в контексте strand
        co_spawn(client_strand, handle_client(std::move(socket)), detached);
      } catch (std::exception &e) {
        std::cerr << std::format("Ошибка обработки запроса: {}\n", e.what());
      }
    }
  }

  Server(tcp::endpoint endpoint, fs::path dbPath, std::size_t capacity,
         std::chrono::milliseconds delay, std::size_t numThreads)
      : ioContext_(numThreads), signalSet_(ioContext_, SIGINT, SIGTERM),
        work_(ioContext_), endPoint_(endpoint),
        storage_(std::move(dbPath), capacity), delay_(delay),
        numThreads_(numThreads) {
    signalSet_.async_wait([this](auto, auto) { ioContext_.stop(); });
  }

  void run() {
    // Запуск сервера
    co_spawn(ioContext_, listen(), detached);

    for (auto i = 0UL; i < numThreads_; ++i) {
      threads_.create_thread([this]() {
        auto executed = ioContext_.run();
        std::cout << (std::stringstream{}
                      << "В потоке " << boost::this_thread::get_id()
                      << " выполнено " << executed << " задач\n")
                         .str();
      });
    }
    threads_.join_all();
    storage_.serialize();
  }

  ~Server() {}

  boost::thread_group threads_;
  boost::asio::io_context ioContext_;
  boost::asio::signal_set signalSet_;
  boost::asio::io_context::work work_;
  tcp::endpoint endPoint_;
  storage::TicketStorage storage_;
  std::chrono::milliseconds delay_;
  std::size_t numThreads_;
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
    auto endpoint = tcp::endpoint(address, port);
    Server(endpoint, dbPath, capacity, delay, 4).run();
  } catch (std::exception &e) {
    std::cerr << std::format("Проблема: {}\n", e.what());
  }

  return 0;
}
