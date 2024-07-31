#include "TcpSocket.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

namespace tcp_socket {

TcpSocket::TcpSocket(int backLogSize) : mBackLogSize(backLogSize) {
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
  std::memset(&addr4in, 0, sizeof(addr4in));
  addr4in.sin_family = AF_INET;
  addr4in.sin_port = ::htons(port);
  addr4in.sin_addr.s_addr = inet_addr(ipAddress.c_str());
  struct ::sockaddr *addr4 = reinterpret_cast<struct ::sockaddr *>(&addr4in);
  if (auto error = ::bind(mFileDescriptor, addr4, sizeof(struct sockaddr_in));
      error)
    throw std::string(::strerror(errno));
}

void TcpSocket::listen() {
  // FIXME: verify that the back log size is reachable
  if (auto status = ::listen(mFileDescriptor, mBackLogSize); status != 0)
    throw std::string(::strerror(errno));
}

} // namespace tcp_socket