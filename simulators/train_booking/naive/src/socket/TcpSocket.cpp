#include "TcpSocket.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

namespace tcp_socket {

TcpConnection::~TcpConnection() { ::close(mFileDescriptor); }

void TcpConnection::dump() const {
  std::cout << "Client IP: " << mAddress << std::endl;
  std::cout << "Client Port: " << mPort << std::endl;
}

std::string TcpConnection::read() {
  char buffer[1024];
  auto bytesRead = ::read(mFileDescriptor, buffer, sizeof buffer);
  return std::string(buffer, bytesRead);
}

void TcpConnection::send(std::string buffer) {
  if (buffer.empty()) {
    throw std::invalid_argument("Cannot send empty string");
  }
  auto bytesSend = ::send(mFileDescriptor, buffer.data(), buffer.size(), 0);
  if (bytesSend != buffer.size()) {
    throw std::out_of_range("Wrong number of bytes sent to the client");
  }
  if (bytesSend < 0) {
    throw TcpSocketError(errno);
  }
}

TcpSocket::TcpSocket() {
  if (mFileDescriptor =
          ::socket(/*domain=ip4*/ AF_INET, /*type=tcp*/ SOCK_STREAM,
                   /*default*/ 0);
      mFileDescriptor == -1)
    throw TcpSocketError(errno);
}

TcpSocket::~TcpSocket() noexcept {
  if (auto error = ::close(mFileDescriptor); error)
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
  if (auto error = ::bind(mFileDescriptor, addr, sizeof(struct sockaddr_in));
      error)
    throw TcpSocketError(errno);
  return *this;
}

TcpSocket &TcpSocket::listen(int backLogSize) {
  // FIXME: verify that the back log size is reachable
  if (auto status = ::listen(mFileDescriptor, backLogSize); status != 0)
    throw TcpSocketError(errno);
  return *this;
}

std::unique_ptr<TcpConnection> TcpSocket::accept() {
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  struct ::sockaddr *addr = reinterpret_cast<struct ::sockaddr *>(&clientAddr);
  int connFileDesc = ::accept(mFileDescriptor, addr, &clientAddrLen);
  if (connFileDesc == -1)
    throw TcpSocketError(errno);
  char clientIP[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
  auto clientPort = ntohs(clientAddr.sin_port);
  return std::make_unique<TcpConnection>(connFileDesc, clientIP, clientPort);
}

} // namespace tcp_socket
