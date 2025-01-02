#include "Exchange.hpp"
#include "Storage.hpp"
#include "TcpSocket.hpp"
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace po = boost::program_options;
namespace fs = std::filesystem;
using namespace std::chrono_literals;

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
    auto address = varMap["address"].as<std::string>();
    auto maxBacklog = varMap["max-backlog"].as<uint16_t>();
    fs::path dbPath = varMap["db-path"].as<std::string>();
    auto capacity = varMap["capacity"].as<std::size_t>();
    auto delay = std::chrono::milliseconds(varMap["delay"].as<std::uint64_t>());

    std::cout << "Порт: " << port << std::endl;
    std::cout << "Адрес: " << address << std::endl;
    std::cout << "Бэклог: " << maxBacklog << std::endl;
    std::cout << "Свободных мест: " << capacity << std::endl;
    std::cout << "Путь к базе данных: " << dbPath << std::endl;

    storage::TicketStorage storage(dbPath, capacity);
    tcp_socket::TcpSocket socket;
    // TODO: нужно вынести в опции командной строки
    boost::asio::thread_pool pool(4);
    const auto &executor = pool.get_executor();
    executor.on_work_started();
    // TODO: нужно всегда закрывать сокет, иначе можно не переподключиться...
    socket.bind(address, port);
    socket.listen(maxBacklog);
    std::cout << "Жду входящих сообщений...\n";
    for (;;) {
      if (!storage.hasVacantSeats()) {
        std::cout << "Свободных мест больше нет\n";
        executor.on_work_finished();
        break;
      }
      if (!socket.checkIncomingConnections(10ms)) {
        continue;
      }
      auto conn = socket.accept();
      boost::asio::post(pool, [&storage, delay, conn = std::move(conn)]() {
        std::string message = conn->read();
        exchange::Request req;
        req.ParseFromString(message);
        auto &&uuid = req.uuid();
        exchange::Response resp;
        resp.set_uuid(uuid);
        if (auto ticket = storage.tryBook(uuid); ticket.has_value()) {
          auto ok = new exchange::Success{};
          ok->set_ticket_num(ticket.value());
          resp.set_allocated_ok(ok);
          if (delay.count()) {
            std::this_thread::sleep_for(delay);
          }
        } else {
          resp.set_allocated_done(new exchange::Done{});
        }
        std::cout << "Новый ответ: " << resp.DebugString() << '\n';
        std::string msg = resp.SerializeAsString();
        if (msg.empty()) {
          exchange::Error error;
          error.set_what("Ошибка сериализации");
          msg = error.SerializeAsString();
        }
        conn->send(std::move(msg));
      });
    }
    // Ждём завершения всех тасок
    pool.join();
    // Теперь можем сериализовать нашу "базу данных" и завершать работу
    storage.serialize();
  } catch (tcp_socket::TcpSocketError &err) {
    std::cerr << "Ошибка TCP соединения, err=" << err.what() << std::endl;
  } catch (...) {
    std::cerr << "Неизвестная ошибка, отключаюсь" << std::endl;
  }
  return EXIT_SUCCESS;
}
