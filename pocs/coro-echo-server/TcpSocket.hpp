#pragma once

#include <cstdint>
#include <expected>
#include <functional>
#include <span>

namespace cpplay {

struct IpAddress;

enum class TcpSocketError { ACCEPT_FAILED, POLL_FAILED, RECV_FAILED };

struct TcpSocket {
  struct Exception;
  TcpSocket();
  TcpSocket(int fd);
  ~TcpSocket();
  void bindTo(const IpAddress &ipAddress);
  void acceptConneciton(
      std::function<void(std::expected<int, TcpSocketError>)> callback);
  void
  readSome(std::span<uint8_t> buffer,
           std::function<void(std::expected<size_t, TcpSocketError>)> callback);
  int getDescriptor() const;

private:
  int mSocketFd{-1};
};
} // namespace cpplay