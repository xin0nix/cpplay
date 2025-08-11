#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <thread>

#include "Coro.hpp"
#include "IpAddress.hpp"
#include "TcpSocket.hpp"

using namespace std::chrono_literals;

cpplay::CoroTask read1(std::shared_ptr<cpplay::Context> context,
                       std::shared_ptr<cpplay::TcpSocket> socket) {
  auto connected = co_await cpplay::AvaitableTcpSocketAcceptor(context, socket);
  std::vector<uint8_t> buffer(128, 0);
  auto bytesRead = co_await cpplay::AvaitableTcpSocketReader(
      context, connected.value(), buffer);
  std::cout << "Bytes read: " << bytesRead.value() << std::endl;
}

int main() {
  cpplay::IpAddress ipAddress({127, 0, 0, 1}, 8080);
  auto tcpSocket = std::make_shared<cpplay::TcpSocket>();
  tcpSocket->bindTo(ipAddress);
  auto context = std::make_shared<cpplay::Context>();
  read1(context, tcpSocket);
}