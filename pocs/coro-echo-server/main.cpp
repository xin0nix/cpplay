#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <future>
#include <iostream>
#include <memory>

#include "Coro.hpp"
#include "IpAddress.hpp"
#include "TcpSocket.hpp"

using namespace std::chrono_literals;

namespace cpplay {
struct Server {

  cpplay::CoroTask echo(std::shared_ptr<cpplay::Context> context,
                        std::shared_ptr<cpplay::TcpSocket> socket, int times) {
    for (int i = 0; i < times; ++i) {
      auto connected =
          co_await cpplay::AvaitableTcpSocketAcceptor(context, socket);
      std::vector<uint8_t> buffer(128, 0);
      if (!connected.has_value()) {
        std::cerr << "Failed to accept connection\n";
        co_return;
      }
      auto bytesRead = co_await cpplay::AvaitableTcpSocketReader(
          context, connected.value(), buffer);
      if (!bytesRead.has_value()) {
        std::cerr << "Failed to read any bytes\n";
        co_return;
      }
      std::cout << "Bytes read: " << bytesRead.value() << std::endl;
      std::unique_lock lock(mMutex);
    }
    mFinished.set_value();
  }

  void wait() {
    std::future<void> f;
    {
      std::cerr << "Waiting for the coroutine to finish\n";
      std::unique_lock lock(mMutex);
      f = mFinished.get_future();
    }
    f.wait();
  }

private:
  std::mutex mMutex;
  std::promise<void> mFinished;
};
} // namespace cpplay

int main() {
  cpplay::IpAddress ipAddress({127, 0, 0, 1}, 8080);
  auto tcpSocket = std::make_shared<cpplay::TcpSocket>();
  tcpSocket->bindTo(ipAddress);
  auto context = std::make_shared<cpplay::Context>();
  cpplay::Server server;
  server.echo(context, tcpSocket, 2);
  server.wait();
  std::cerr << "Exiting from main\n";
}