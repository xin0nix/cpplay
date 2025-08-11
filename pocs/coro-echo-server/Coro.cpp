#include "Coro.hpp"
#include <expected>

namespace cpplay {

// То что будет возвращено вызывающей стороне
CoroTask CoroPromise::get_return_object() {
  return {CoroTask::from_promise(*this)};
}
std::suspend_never CoroPromise::initial_suspend() noexcept { return {}; }
std::suspend_never CoroPromise::final_suspend() noexcept { return {}; }
void CoroPromise::return_void() {}
void CoroPromise::unhandled_exception() {}

//-----------------------------------------------------------------------------
// Connection Acceptor

AvaitableTcpSocketAcceptor::AvaitableTcpSocketAcceptor(
    std::shared_ptr<Context> context, std::shared_ptr<TcpSocket> socket)
    : mContext(std::move(context)), mTcpSocket(std::move(socket)) {}

bool AvaitableTcpSocketAcceptor::await_ready() const noexcept { return false; }

void AvaitableTcpSocketAcceptor::await_suspend(
    std::coroutine_handle<CoroPromise> handle) noexcept {
  mContext->mPool.enqueue([this, handle]() {
    mTcpSocket->acceptConneciton(
        [this, handle](std::expected<int, TcpSocketError> socketFd) {
          if (socketFd.has_value()) {
            this->mConnectedSocket =
                std::make_shared<TcpSocket>(socketFd.value());
          } else {
            this->mConnectedSocket = std::unexpected(socketFd.error());
          }
          this->mContext->mLoop.postTask([handle]() { handle.resume(); });
        });
  });
}

std::expected<std::shared_ptr<TcpSocket>, TcpSocketError>
AvaitableTcpSocketAcceptor::await_resume() const noexcept {
  return this->mConnectedSocket;
}

//-----------------------------------------------------------------------------
// Socket Reader

AvaitableTcpSocketReader::AvaitableTcpSocketReader(
    std::shared_ptr<Context> context, std::shared_ptr<TcpSocket> socket,
    std::span<uint8_t> buffer)
    : mContext(std::move(context)), mTcpSocket(std::move(socket)),
      mBuffer(buffer) {}

bool AvaitableTcpSocketReader::await_ready() const noexcept { return false; }

void AvaitableTcpSocketReader::AvaitableTcpSocketReader::await_suspend(
    std::coroutine_handle<CoroPromise> handle) noexcept {
  mTcpSocket->acceptConneciton(
      [this, handle](std::expected<size_t, TcpSocketError> bytesRead) {
        this->mBytesRead = bytesRead;
        this->mContext->mLoop.postTask([handle]() { handle.resume(); });
      });
}

std::expected<size_t, TcpSocketError>
AvaitableTcpSocketReader::await_resume() const noexcept {
  return mBytesRead;
}
} // namespace cpplay