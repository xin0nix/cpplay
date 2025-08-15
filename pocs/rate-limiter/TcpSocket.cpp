#include "TcpSocket.hpp"

#include "IpAddress.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace cpplay {
TcpSocket::Exception::Exception(int errorNumber)
    : mMessage(strerror(errorNumber)) {
  std::cerr << "TcpSocket::Exception created with error number: " << errorNumber
            << "\n";
}

TcpSocket::Exception::Exception(std::string errorMessage)
    : mMessage(std::move(errorMessage)) {
  std::cerr << "TcpSocket::Exception created with error message: "
            << errorMessage << "\n";
}

const char *TcpSocket::Exception::what() const noexcept {
  return mMessage.c_str();
}

TcpSocket::TcpSocket() {
  std::cerr << "TcpSocket: Creating socket\n";
  // FIXME: handle SIGPIPE somehow?
  mSocketFd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (mSocketFd == -1) {
    std::cerr << "TcpSocket: Socket creation failed with errno " << errno
              << " (" << strerror(errno) << ")\n";
    throw Exception(errno);
  }
  std::cerr << "TcpSocket: Socket created with fd " << mSocketFd << "\n";
}

TcpSocket::TcpSocket(int fd) : mSocketFd(fd) {
  std::cerr << "TcpSocket: Wrapped existing socket fd " << fd << "\n";
}

int TcpSocket::getDescriptor() const {
  std::cerr << "TcpSocket: getDescriptor called, returning " << mSocketFd
            << "\n";
  return mSocketFd;
}

TcpSocket::~TcpSocket() {
  std::cerr << "TcpSocket: Closing socket fd " << mSocketFd << "\n";
  ::close(mSocketFd);
}

void TcpSocket::bindTo(const IpAddress &ipAddress) {
  auto [addr, port] = ipAddress.getFullHostAddress();

  struct sockaddr_in serverAddress;
  // Set up the server address structure
  serverAddress.sin_family = AF_INET;
  std::cerr << "TcpSocket: Binding to " << addr << ":" << port << std::endl;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = inet_addr(addr.c_str());

  // Bind the socket to the address
  if (::bind(mSocketFd, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) == -1) {
    std::cerr << "TcpSocket: Bind failed with errno " << errno << " ("
              << strerror(errno) << ")\n";
    throw Exception(errno);
  }
  int opt = 1;
  if (::setsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
      0) {
    std::cerr << "TcpSocket: setsockopt SO_REUSEADDR failed with errno "
              << errno << " (" << strerror(errno) << ")\n";
    throw Exception(errno);
  }
  if (::listen(mSocketFd, 10) == -1) {
    std::cerr << "TcpSocket: Listen failed with errno " << errno << " ("
              << strerror(errno) << ")\n";
    throw Exception(errno);
  }
  std::cerr << "TcpSocket: Listening started on " << addr << ":" << port
            << "\n";
}

int TcpSocket::acceptConneciton() {
  std::cerr << "TcpSocket: Calling accept\n";
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  int connFd =
      accept(mSocketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
  if (connFd == -1) {
    std::cerr << "TcpSocket: Accept failed with errno " << errno << " ("
              << strerror(errno) << ")\n";
    throw Exception(errno);
  }
  std::cerr << "TcpSocket: Accepted connection, new fd " << connFd << "\n";
  return connFd;
}

size_t TcpSocket::readSome(std::span<uint8_t> buffer) {
  struct pollfd pfd;
  pfd.fd = mSocketFd;
  pfd.events = POLLIN;
  std::cerr << "TcpSocket: Entering readSome loop with 30s timeout\n";
  int pollCount = ::poll(&pfd, 1, 30'000 /*ms*/);
  if (pollCount == -1) {
    std::cerr << "TcpSocket: poll failed with errno " << errno << " ("
              << strerror(errno) << ")\n";
    throw Exception(errno);
  }
  if (pfd.revents & POLLIN) {
    ssize_t n = ::recv(mSocketFd, buffer.data(), buffer.size(), 0);
    if (n > 0) {
      std::cerr << "TcpSocket: Received " << n << " bytes\n";
      return n;
    } else {
      std::cerr << "TcpSocket: recv failed or connection closed with errno "
                << errno << " (" << strerror(errno) << ")\n";
      throw Exception(errno);
    }
  } else {
    std::cerr << "TcpSocket: poll unrelated event " << pfd.revents << std::endl;
    throw Exception("TcpSocket: poll unrelated event ");
  }
}

size_t TcpSocket::writeSome(std::span<uint8_t> buffer) {

  ssize_t n = ::send(mSocketFd, buffer.data(), buffer.size(), 0);
  if (n > 0) {
    std::cerr << "TcpSocket: Sent " << n << " bytes\n";
    return n;
  } else {
    std::cerr << "TcpSocket: send failed or connection closed with errno "
              << errno << " (" << strerror(errno) << ")\n";
    throw Exception(errno);
  }
}

} // namespace cpplay
