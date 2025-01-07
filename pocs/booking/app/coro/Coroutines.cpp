#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>

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

int main() {
  try {
    boost::asio::io_context io_context;

    // Создание акцептора для прослушивания порта 12345
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));

    // Запуск сервера
    co_spawn(io_context, start_server(acceptor), detached);

    // Запуск io_context
    io_context.run();
  } catch (std::exception &e) {
    std::printf("Произошла ошибка: %s\n", e.what());
  }

  return 0;
}
