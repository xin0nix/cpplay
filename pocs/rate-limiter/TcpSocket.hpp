#pragma once

#include <cstdint>
#include <exception>
#include <expected>
#include <functional>
#include <span>
#include <string>

namespace cpplay {

struct IpAddress;

enum class TcpSocketError {
  ACCEPT_FAILED,
  POLL_FAILED,
  RECV_FAILED,
  SEND_FAILED
};

struct TcpSocket final {
  struct Exception : std::exception {
    Exception(int errorNumber);

    Exception(std::string errorMessage);

    const char *what() const noexcept override;

  private:
    std::string mMessage;
  };

  TcpSocket();
  TcpSocket(int fd);
  ~TcpSocket();
  void bindTo(const IpAddress &ipAddress);
  int acceptConneciton();
  void connectTo(const IpAddress &ipAddress);
  size_t readSome(std::span<uint8_t> buffer);
  size_t writeSome(std::span<uint8_t> buffer);
  int getDescriptor() const;

private:
  int mSocketFd{-1};
};
} // namespace cpplay