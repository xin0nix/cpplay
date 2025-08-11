#pragma once

#include "Context.hpp"
#include "TcpSocket.hpp"
#include <coroutine>
#include <expected>
#include <memory>

namespace cpplay {

struct CoroPromise;

struct CoroTask : std::coroutine_handle<CoroPromise> {
  using promise_type = CoroPromise;
};

struct CoroPromise {
  // То что будет возвращено вызывающей стороне
  CoroTask get_return_object();
  // Доходим до первого co_await
  std::suspend_never initial_suspend() noexcept;
  // Сразу выходим
  std::suspend_never final_suspend() noexcept;
  // co_return ничего не возвращает
  void return_void();
  // игнорируем исключения
  void unhandled_exception();
};

struct AvaitableTcpSocketAcceptor {
  explicit AvaitableTcpSocketAcceptor(std::shared_ptr<Context> context,
                                      std::shared_ptr<TcpSocket> socket);
  bool await_ready() const noexcept;
  void await_suspend(std::coroutine_handle<CoroPromise> handle) noexcept;
  std::expected<std::shared_ptr<TcpSocket>, TcpSocketError>
  await_resume() const noexcept;

private:
  std::shared_ptr<Context> mContext;
  std::shared_ptr<TcpSocket> mTcpSocket;
  std::expected<std::shared_ptr<TcpSocket>, TcpSocketError> mConnectedSocket;
};

struct AvaitableTcpSocketReader {
  explicit AvaitableTcpSocketReader(std::shared_ptr<Context> context,
                                    std::shared_ptr<TcpSocket> socket,
                                    std::span<uint8_t> buffer);
  // а нужно ли нам вообще засыпать, может все уже и так готово и мы можем
  // продолжить сразу?
  bool await_ready() const noexcept;

  // здесь мы можем запустить какой-то процесс, по завершению которого нами
  // будет вызван handle.resume()
  void await_suspend(std::coroutine_handle<CoroPromise> handle) noexcept;

  // здесь мы вернем вызывающей стороне результат операции
  std::expected<size_t, TcpSocketError> await_resume() const noexcept;

private:
  std::span<uint8_t> mBuffer;
  std::shared_ptr<TcpSocket> mTcpSocket;
  std::shared_ptr<Context> mContext;
  std::expected<size_t, TcpSocketError> mBytesRead{0};
};

} // namespace cpplay