#include "TcpSocket.hpp"

#include "IpAddress.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace cpplay {
struct TcpSocket::Exception : std::exception {
  Exception(int errorNumber) : mErrorNumber(errorNumber) {}
  const char *what() const noexcept override { return strerror(mErrorNumber); }

private:
  int mErrorNumber;
};

TcpSocket::TcpSocket() {
  // FIXME: handle SIGPIPE somehow?
  mSocketFd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (mSocketFd == -1) {
    throw Exception(errno);
  }
}

TcpSocket::TcpSocket(int fd) : mSocketFd(fd) {}

int TcpSocket::getDescriptor() const { return mSocketFd; }

TcpSocket::~TcpSocket() { ::close(mSocketFd); }

void TcpSocket::bindTo(const IpAddress &ipAddress) {
  auto [addr, port] = ipAddress.getFullHostAddress();

  struct sockaddr_in serverAddress;
  // Set up the server address structure
  serverAddress.sin_family = AF_INET;
  std::cerr << "Listening to " << addr << ":" << port << std::endl;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = inet_addr(addr.c_str());

  // Bind the socket to the address
  if (::bind(mSocketFd, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) == -1) {
    throw Exception(errno);
  }
  int opt = 1;
  if (::setsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
      0) {
    throw Exception(errno);
  }
  if (::listen(mSocketFd, 10) == -1) {
    throw Exception(errno);
  }
}

void TcpSocket::acceptConneciton(
    std::function<void(std::expected<int, TcpSocketError>)> callback) {
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  int connFd =
      accept(mSocketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
  if (connFd < 0) {
    callback(std::unexpected(TcpSocketError::ACCEPT_FAILED));
    return;
  }
  callback(connFd);
}

void TcpSocket::readSome(
    std::span<uint8_t> buffer,
    std::function<void(std::expected<size_t, TcpSocketError>)> callback) {
  struct pollfd pfd;
  pfd.fd = mSocketFd;
  pfd.events = POLLIN;
  while (true) {
    int pollCount = ::poll(&pfd, 1, 30'000 /*ms*/);
    if (pollCount < 0) {
      callback(std::unexpected(TcpSocketError::POLL_FAILED));
      return;
    }
    if (pfd.revents & POLLIN) {
      ssize_t n = ::recv(mSocketFd, buffer.data(), buffer.size(), 0);
      if (n > 0) {
        callback(n);
        return;
      }
      callback(std::unexpected(TcpSocketError::RECV_FAILED));
      return;
    }
  }
}

} // namespace cpplay