#include "TcpSocket.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace tcp_socket {

TcpConnection::~TcpConnection() {
  if (auto error = ::close(socketFileDescriptor_); error)
    std::cerr << ::strerror(errno)
              << std::endl; // FIXME: replace with boost logv2
}

void TcpConnection::dump() const {
  std::cout << "Client IP: " << address_ << std::endl;
  std::cout << "Client Port: " << port_ << std::endl;
}

std::string TcpConnection::read() {
  char buffer[1024];
  auto bytesRead = ::read(socketFileDescriptor_, buffer, sizeof buffer);
  return std::string(buffer, bytesRead);
}

void TcpConnection::send(std::string buffer) {
  if (buffer.empty()) {
    throw std::invalid_argument("Cannot send empty string");
  }
  auto bytesSend =
      ::send(socketFileDescriptor_, buffer.data(), buffer.size(), 0);
  if (bytesSend != buffer.size()) {
    throw std::out_of_range("Wrong number of bytes sent to the client");
  }
  if (bytesSend < 0) {
    throw TcpSocketError(errno);
  }
}

TcpSocket::TcpSocket() {
  if (socketFileDescriptor_ =
          ::socket(/*domain=ip4*/ AF_INET, /*type=tcp*/ SOCK_STREAM,
                   /*default*/ 0);
      socketFileDescriptor_ == -1)
    throw TcpSocketError(errno);
}

TcpSocket::~TcpSocket() noexcept {
  if (auto error = ::close(socketFileDescriptor_); error)
    std::cerr << ::strerror(errno)
              << std::endl; // FIXME: replace with boost logv2
}

TcpSocket &TcpSocket::bind(std::string ipAddress, const uint16_t port) {
  struct ::sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = ::htons(port);
  serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
  struct ::sockaddr *addr = reinterpret_cast<struct ::sockaddr *>(&serverAddr);
  if (auto error =
          ::bind(socketFileDescriptor_, addr, sizeof(struct sockaddr_in));
      error)
    throw TcpSocketError(errno);
  return *this;
}

TcpSocket &TcpSocket::listen(int backLogCapacity) {
  // FIXME: verify that the back log size is reachable
  if (auto status = ::listen(socketFileDescriptor_, backLogCapacity);
      status != 0)
    throw TcpSocketError(errno);
  return *this;
}

bool TcpSocket::checkIncomingConnections(std::chrono::milliseconds timeout) {
  ::pollfd fds{
      .fd = socketFileDescriptor_,
      .events = POLLIN, // Мониторим входящие запросы
      .revents = 0,
  };
  ::nfds_t fdsSize = 1;
  int status = ::poll(&fds, fdsSize, timeout.count());
  if (status == 0) {
    // Сработал таймаут, никаких событий
    return false;
  }
  if (status == -1) {
    // Ошибка не связанная напрямую с TCP (условно)
    throw TcpSocketError(errno);
  }
  if (fds.revents) {
    if (status & POLLIN) {
      return true;
    }
    if (status & POLLERR or status & POLLHUP) {
      throw TcpSocketError("Ошибка TCP соединения, статус=" +
                           std::to_string(status));
    }
    throw TcpSocketError("Неизвестная ошибка уровня TCP, статус=" +
                         std::to_string(status));
  }
  return false;
}

std::unique_ptr<TcpConnection> TcpSocket::accept() {
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  struct ::sockaddr *addr = reinterpret_cast<struct ::sockaddr *>(&clientAddr);
  int connFileDesc = ::accept(socketFileDescriptor_, addr, &clientAddrLen);
  if (connFileDesc == -1)
    throw TcpSocketError(errno);
  char clientIP[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
  auto clientPort = ntohs(clientAddr.sin_port);
  return std::make_unique<TcpConnection>(connFileDesc, clientIP, clientPort);
}

} // namespace tcp_socket
