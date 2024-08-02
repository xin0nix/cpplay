#include "TcpSocket.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

namespace tcp_socket {
TcpConnection::TcpConnection(int fileDescriptor)
    : mFileDescriptor(fileDescriptor) {}

TcpConnection::~TcpConnection() { ::close(mFileDescriptor); }

TcpSocket::TcpSocket() {
  if (mFileDescriptor =
          ::socket(/*domain=ip4*/ AF_INET, /*type=tcp*/ SOCK_STREAM,
                   /*default*/ 0);
      mFileDescriptor == -1)
    throw std::string(::strerror(errno));
}

TcpSocket::~TcpSocket() noexcept {
  if (auto error = ::close(mFileDescriptor); error)
    std::cerr << ::strerror(errno)
              << std::endl; // FIXME: replace with boost logv2
}

void TcpSocket::bind(std::string ipAddress, const uint16_t port) {
  struct ::sockaddr_in addr4in;
  memset(&addr4in, 0, sizeof(addr4in));
  addr4in.sin_family = AF_INET;
  addr4in.sin_port = ::htons(port);
  addr4in.sin_addr.s_addr = inet_addr(ipAddress.c_str());
  struct ::sockaddr *addr4 = reinterpret_cast<struct ::sockaddr *>(&addr4in);
  if (auto error = ::bind(mFileDescriptor, addr4, sizeof(struct sockaddr_in));
      error)
    throw std::string(::strerror(errno));
}

void TcpSocket::listen(int backLogSize) {
  // FIXME: verify that the back log size is reachable
  if (auto status = ::listen(mFileDescriptor, backLogSize); status != 0)
    throw std::string(::strerror(errno));
}

std::unique_ptr<TcpConnection> TcpSocket::accept() {
  int connFileDescriptor = ::accept(mFileDescriptor, nullptr, nullptr);
  if (connFileDescriptor == -1)
    throw std::string(::strerror(errno));
  return std::make_unique<TcpConnection>(connFileDescriptor);
}

} // namespace tcp_socket
