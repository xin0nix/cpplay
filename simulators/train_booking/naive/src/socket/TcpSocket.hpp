#pragma once

#include <memory>
#include <string>

namespace tcp_socket {
struct TcpConnection final {
  ~TcpConnection();

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
