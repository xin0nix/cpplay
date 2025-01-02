#pragma once

#include <chrono>
#include <exception>
#include <memory>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace tcp_socket {

class TcpSocketError : public std::exception {
public:
  explicit TcpSocketError(int errorCode)
      : errorMessage_(::strerror(errorCode)) {}

  explicit TcpSocketError(std::string errorMessage)
      : errorMessage_(std::move(errorMessage)) {}

  const char *what() const throw() override { return errorMessage_.c_str(); }

private:
  std::string errorMessage_;
};

struct TcpConnection final {
  TcpConnection(int fileDesc, std::string addr, uint16_t port)
      : socketFileDescriptor_(fileDesc), address_(addr), port_(port) {}
  ~TcpConnection();

  /// Вывести на экран отладочную информацию о подключении
  void dump() const;
  // TODO: нужно переделать через stream
  /// Прочесть данные (до 1024 байт)
  std::string read();
  /// Отправить данные
  void send(std::string buffer);

private:
  /// Хранит дескриптор файла, связанный с подключением
  // TODO: заменить на optional
  int socketFileDescriptor_{-1};
  std::string address_;
  const uint16_t port_;
};

struct TcpSocket final {
  /// Создёт новое TCP подключение по сети IP
  TcpSocket();
  /// Закрывает файловый дескриптор, ассоциированный с TCP сокетом
  ~TcpSocket() noexcept;
  /// Связывает сокет с IP адресом и портом
  TcpSocket &bind(std::string ipAddress, const uint16_t port);
  /// Начать слушать входные соединения
  TcpSocket &listen(int backLogCapacity);
  /// Проверить наличие входящих запросов на подключение
  bool checkIncomingConnections(std::chrono::milliseconds timeout);
  /// Принять входящее подключение и начать принимать данные
  std::unique_ptr<TcpConnection> accept();

private:
  /// File descriptor associated with the socket
  int socketFileDescriptor_{-1};
};
} // namespace tcp_socket
