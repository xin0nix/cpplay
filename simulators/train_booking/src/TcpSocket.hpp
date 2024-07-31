#pragma once

#include <string>

namespace tcp_socket {
struct TcpSocket final {
  /// Create a new TCP socket over IP network
  TcpSocket(int backLogSize);
  /// Close the fd associated with the TCP socket
  ~TcpSocket() noexcept;
  /// Assign a name to a socket
  void bind(std::string ipAddress, const uint16_t port);
  /// Mark socket as a listening (passive) one
  void listen();

private:
  /// File descriptor associated with the socket
  int mFileDescriptor{-1};

  /// Backlog size, i.e. the maximum number of pending connections that can be
  /// queued for a socket before the kernel starts refusing new connections
  int mBackLogSize{0};
};
} // namespace tcp_socket
