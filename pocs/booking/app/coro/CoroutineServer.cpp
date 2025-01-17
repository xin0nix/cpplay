#include "CoroutineServer.hpp"

#include "Exchange.hpp"
#include "Exchange.pb.h"

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/this_coro.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <exception>
#include <format>
#include <iostream>
#include <sstream>
#include <string>

namespace rng = std::ranges;
using namespace std::chrono_literals;

using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::ip::tcp;

// Корутина для обработки клиентского подключения
awaitable<void> CoroutineServer::handle_client(tcp::socket socket) {
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
#ifdef DEBUG
    std::cerr << std::format("Получено новое сообщение: {}\n",
                             request.DebugString());
#endif
    auto &&uuid = request.uuid();
    exchange::Response resp;
    resp.set_uuid(uuid);
    if (auto ticket = storage_.tryBook(uuid); ticket.has_value()) {
      auto ok = new exchange::Success{};
      ok->set_ticket_num(ticket.value());
      resp.set_allocated_ok(ok);
      if (fakeDelay_.count()) {
        auto executor = co_await boost::asio::this_coro::executor;
        boost::asio::steady_timer timer(executor);
        timer.expires_after(fakeDelay_);
        co_await timer.async_wait(use_awaitable);
      }
    } else {
      resp.set_allocated_done(new exchange::Done{});
      done = true;
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
    // TODO: запись в цикле
    co_await async_write(socket, boost::asio::buffer(msg), use_awaitable);
  }
  // Это нормально? Успеют ли остальные корутины отработать??
  ioContext_.stop();
}

// Корутина основного цикла сервера
awaitable<void> CoroutineServer::listen() {
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

#ifdef DEBUG
      auto client = socket.remote_endpoint();
      std::cout << std::format("Подключился клиент, IP: {}, Порт: {}\n",
                               client.address().to_string(), client.port());
#endif

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

void CoroutineServer::serve() {
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
