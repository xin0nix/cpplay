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
  CoroTask get_return_object();
  std::suspend_never initial_suspend() noexcept;
  std::suspend_never final_suspend() noexcept;
  void return_void();
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

  bool await_ready() const noexcept;

  void await_suspend(std::coroutine_handle<CoroPromise> handle) noexcept;

  std::expected<size_t, TcpSocketError> await_resume() const noexcept;

private:
  std::span<uint8_t> mBuffer;
  std::shared_ptr<TcpSocket> mTcpSocket;
  std::shared_ptr<Context> mContext;
  std::expected<size_t, TcpSocketError> mBytesRead{0};
};

struct AvaitableTcpSocketWriter {
  explicit AvaitableTcpSocketWriter(std::shared_ptr<Context> context,
                                    std::shared_ptr<TcpSocket> socket,
                                    std::span<uint8_t> buffer);
  bool await_ready() const noexcept;

  void await_suspend(std::coroutine_handle<CoroPromise> handle) noexcept;

  std::expected<size_t, TcpSocketError> await_resume() const noexcept;

private:
  std::span<uint8_t> mBuffer;
  std::shared_ptr<TcpSocket> mTcpSocket;
  std::shared_ptr<Context> mContext;
  std::expected<size_t, TcpSocketError> mBytesRead{0};
};

} // namespace cpplay