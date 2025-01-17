#include "ThreadPoolServer.hpp"

#include "Exchange.hpp"
#include "Storage.hpp"
#include "TcpSocket.hpp"

#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

using namespace std::chrono_literals;

void ThreadPoolServer::serve() {
  try {
    tcp_socket::TcpSocket socket;
    boost::asio::thread_pool pool(numThreads_);
    const auto &executor = pool.get_executor();
    executor.on_work_started();
    // TODO: нужно всегда закрывать сокет, иначе можно не переподключиться...
    socket.bind(endPoint_.address().to_string(), endPoint_.port());
    socket.listen(maxBacklogSize_);
#ifdef DEBUG
    std::cout << "Жду входящих сообщений...\n";
#endif
    for (;;) {
      if (!storage_.hasVacantSeats()) {
        std::cout << "Свободных мест больше нет\n";
        executor.on_work_finished();
        break;
      }
      if (!socket.checkIncomingConnections(10ms)) {
        continue;
      }
      auto conn = socket.accept();
      // TODO: постить с захватом this опасно, storage нужно передавать как
      // shared pointer
      boost::asio::post(
          pool, [this, delay = fakeDelay_, conn = std::move(conn)]() {
            std::string message = conn->read();
            exchange::Request req;
            req.ParseFromString(message);
            auto &&uuid = req.uuid();
            exchange::Response resp;
            resp.set_uuid(uuid);
            if (auto ticket = storage_.tryBook(uuid); ticket.has_value()) {
              auto ok = new exchange::Success{};
              ok->set_ticket_num(ticket.value());
              resp.set_allocated_ok(ok);
              if (delay.count()) {
                std::this_thread::sleep_for(delay);
              }
            } else {
              resp.set_allocated_done(new exchange::Done{});
            }
#ifdef DEBUG
            std::cout << "Новый ответ: " << resp.DebugString() << '\n';
#endif
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
    storage_.serialize();
  } catch (tcp_socket::TcpSocketError &err) {
    std::cerr << "Ошибка TCP соединения, err=" << err.what() << std::endl;
  } catch (...) {
    std::cerr << "Неизвестная ошибка, отключаюсь" << std::endl;
  }
}
