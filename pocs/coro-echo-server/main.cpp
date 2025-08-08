#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <experimental/scope>
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
    auto guard = std::experimental::scope_exit{[this] {
      std::cerr << "Setting echo promise\n";
      mEchoPromise.set_value();
    }};
    for (int i = 0; i < times; ++i) {
      auto connected =
          co_await cpplay::AvaitableTcpSocketAcceptor(context, socket);
      if (!connected.has_value()) {
        std::cerr << "Failed to accept connection\n";
        co_return;
      }
      auto connection = std::move(connected).value();
      std::vector<uint8_t> buffer(4096, 0);
      auto readRes = co_await cpplay::AvaitableTcpSocketReader(
          context, connection, buffer);
      if (!readRes.has_value()) {
        std::cerr << "Failed to read any bytes\n";
        co_return;
      }
      auto received = readRes.value();
      std::cout << "Bytes read: " << received << std::endl;
      std::span sendBuff(buffer.begin(), buffer.begin() + received);
      auto bytesSent = co_await cpplay::AvaitableTcpSocketWriter(
          context, connection, sendBuff);
      if (!bytesSent.has_value()) {
        std::cerr << "Failed to send any bytes\n";
        co_return;
      }
      auto sent = bytesSent.value();
      if (sent != received) {
        std::cerr << "Mismatch, exiting, sent=" << sent
                  << ", received=" << received << std::endl;
        co_return;
      }
    }
  }

  void wait() {
    std::cerr << "Waiting for the echo to finish\n";
    mEchoPromise.get_future().wait();
  }

private:
  std::promise<void> mEchoPromise;
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