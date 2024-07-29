#pragma once

namespace tcp_socket {
struct TcpSocket {
  /// Create a new TCP socket over IP network
  TcpSocket();

private:
  /// File descriptor associated with the socket
  int mFileDescriptor;
};
} // namespace tcp_socket
