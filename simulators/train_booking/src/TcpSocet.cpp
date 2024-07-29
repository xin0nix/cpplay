#include "TcpSocket.hpp"

#include <sys/socket.h>

namespace tcp_socket {
TcpSocket::TcpSocket() {
  mFileDescriptor = socket(/*ip4*/ AF_INET, /*tcp*/ SOCK_STREAM, /*default*/ 0);
}
} // namespace tcp_socket
