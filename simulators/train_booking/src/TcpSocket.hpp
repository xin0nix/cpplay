#pragma once

#include <string>

namespace tcp_socket {
struct TcpSocket final {
  /// Create a new TCP socket over IP network
  TcpSocket();
  /// Close the fd associated with the TCP socket
  ~TcpSocket() noexcept;
  /// Assign a name to a socket
  void bind(std::string ipAddress, const uint16_t port);

private:
  /// File descriptor associated with the socket
  int mFileDescriptor;
};
} // namespace tcp_socket
