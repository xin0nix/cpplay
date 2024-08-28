#pragma once

#include <exception>
#include <memory>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace tcp_socket {

class TcpSocketError : public std::exception {
public:
  TcpSocketError(int errorCode) : errorCode_(errorCode) {}

  int errorCode() const { return errorCode_; }

  const char *what() const throw() override { return ::strerror(errorCode_); }

private:
  int errorCode_;
};

struct TcpConnection final {
  TcpConnection(int fileDesc, std::string addr, uint16_t port)
      : mFileDescriptor(fileDesc), mAddress(addr), mPort(port) {}
  ~TcpConnection();

  void dump() const;
  std::string read();
  void send(std::string buffer);

private:
  /// File descriptor associated with this connection
  int mFileDescriptor{-1};
  std::string mAddress;
  const uint16_t mPort;
};

struct TcpSocket final {
  /// Create a new TCP socket over IP network
  TcpSocket();
  /// Close the fd associated with the TCP socket
  ~TcpSocket() noexcept;
  /// Assign a name to a socket
  TcpSocket &bind(std::string ipAddress, const uint16_t port);
  /// Mark socket as a listening (passive) one
  TcpSocket &listen(int backLogSize);
  /// Extract the first socket from the backlog and return the connection
  std::unique_ptr<TcpConnection> accept();

private:
  /// File descriptor associated with the socket
  int mFileDescriptor{-1};
};
} // namespace tcp_socket
