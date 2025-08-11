#include "Coro.hpp"
#include <expected>
#include <iostream> // Added for std::cerr

namespace cpplay {

// То что будет возвращено вызывающей стороне
CoroTask CoroPromise::get_return_object() {
  std::cerr << "CoroPromise: get_return_object called\n";
  return {CoroTask::from_promise(*this)};
}
std::suspend_never CoroPromise::initial_suspend() noexcept {
  std::cerr << "CoroPromise: initial_suspend called\n";
  return {};
}
std::suspend_never CoroPromise::final_suspend() noexcept {
  std::cerr << "CoroPromise: final_suspend called\n";
  return {};
}
void CoroPromise::return_void() {
  std::cerr << "CoroPromise: return_void called\n";
}
void CoroPromise::unhandled_exception() {
  std::cerr << "CoroPromise: unhandled_exception called\n";
}

//-----------------------------------------------------------------------------
// Connection Acceptor

AvaitableTcpSocketAcceptor::AvaitableTcpSocketAcceptor(
    std::shared_ptr<Context> context, std::shared_ptr<TcpSocket> socket)
    : mContext(std::move(context)), mTcpSocket(std::move(socket)) {
  std::cerr << "AvaitableTcpSocketAcceptor: Constructor called\n";
}

bool AvaitableTcpSocketAcceptor::await_ready() const noexcept {
  std::cerr
      << "AvaitableTcpSocketAcceptor: await_ready called, returning false\n";
  return false;
}

void AvaitableTcpSocketAcceptor::await_suspend(
    std::coroutine_handle<CoroPromise> handle) noexcept {
  std::cerr << "AvaitableTcpSocketAcceptor: await_suspend called, enqueuing "
               "accept task\n";
  mContext->mPool.enqueue([this, handle]() {
    std::cerr << "AvaitableTcpSocketAcceptor: Accept connection started\n";
    mTcpSocket->acceptConneciton([this,
                                  handle](std::expected<int, TcpSocketError>
                                              socketFd) {
      if (socketFd.has_value()) {
        std::cerr
            << "AvaitableTcpSocketAcceptor: Connection accepted, socketFd: "
            << socketFd.value() << "\n";
        this->mConnectedSocket = std::make_shared<TcpSocket>(socketFd.value());
      } else {
        std::cerr << "AvaitableTcpSocketAcceptor: Connection accept failed\n";
        this->mConnectedSocket = std::unexpected(socketFd.error());
      }
      std::cerr
          << "AvaitableTcpSocketAcceptor: Posting resume task to event loop\n";
      this->mContext->mLoop.postTask([handle]() {
        std::cerr << "AvaitableTcpSocketAcceptor: Resuming coroutine\n";
        handle.resume();
      });
    });
  });
}

std::expected<std::shared_ptr<TcpSocket>, TcpSocketError>
AvaitableTcpSocketAcceptor::await_resume() const noexcept {
  std::cerr << "AvaitableTcpSocketAcceptor: await_resume called\n";
  return this->mConnectedSocket;
}

//-----------------------------------------------------------------------------
// Socket Reader

AvaitableTcpSocketReader::AvaitableTcpSocketReader(
    std::shared_ptr<Context> context, std::shared_ptr<TcpSocket> socket,
    std::span<uint8_t> buffer)
    : mContext(std::move(context)), mTcpSocket(std::move(socket)),
      mBuffer(buffer) {
  std::cerr << "AvaitableTcpSocketReader: Constructor called\n";
}

bool AvaitableTcpSocketReader::await_ready() const noexcept {
  std::cerr
      << "AvaitableTcpSocketReader: await_ready called, returning false\n";
  return false;
}

void AvaitableTcpSocketReader::await_suspend(
    std::coroutine_handle<CoroPromise> handle) noexcept {
  std::cerr
      << "AvaitableTcpSocketReader: await_suspend called, starting read\n";
  mTcpSocket->readSome(
      mBuffer, [this, handle](std::expected<size_t, TcpSocketError> bytesRead) {
        if (bytesRead.has_value()) {
          std::cerr << "AvaitableTcpSocketReader: Bytes read: "
                    << bytesRead.value() << "\n";
        } else {
          std::cerr << "AvaitableTcpSocketReader: Read failed\n";
        }
        this->mBytesRead = bytesRead;
        std::cerr
            << "AvaitableTcpSocketReader: Posting resume task to event loop\n";
        this->mContext->mLoop.postTask([handle]() {
          std::cerr << "AvaitableTcpSocketReader: Resuming coroutine\n";
          handle.resume();
        });
      });
}

std::expected<size_t, TcpSocketError>
AvaitableTcpSocketReader::await_resume() const noexcept {
  std::cerr << "AvaitableTcpSocketReader: await_resume called\n";
  return mBytesRead;
}

} // namespace cpplay
