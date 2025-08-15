#include "IpAddress.hpp"
#include "TcpSocket.hpp"
#include "ThreadPool.hpp"

#include <atomic>
#include <cstdint>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace cpplay {
struct RateLimiter {
  RateLimiter(IpAddress hostAddress, IpAddress apiAddress)
      : mHostAddress(hostAddress), mApiAddress(apiAddress),
        mTokenThread(&RateLimiter::tokenSupplier, this) {}

  ~RateLimiter() {
    if (mTokenThread.joinable()) {
      mTokenThread.join();
    }
  }

  void tokenSupplier() {
    while (!mStopFlag.load(std::memory_order_acquire)) {
      std::this_thread::sleep_for(10s);
      mTokenBucket.store(1, std::memory_order_release);
    }
  }

  void run() {
    // FIXME: infinite loop or something
    TcpSocket acceptor;
    acceptor.bindTo(mHostAddress);
    for (int i = 0; i < 10; ++i) {
      // Accept a connection
      try {
        auto connectionFd = acceptor.acceptConneciton();
        mWorkerPool.enqueue([this, connectionFd]() {
          TcpSocket client{connectionFd};
          auto token = --mTokenBucket;
          if (token < 0) {
            // Reject
            std::vector<uint8_t> bad = {0U};
            client.writeSome(bad);
          } else {
            // Propagate
            TcpSocket api;
            api.connectTo(mApiAddress);
            std::vector<uint8_t> buffer(4096, 0);
            api.readSome(buffer);
            client.writeSome(buffer);
          }
        });
      } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
      }
    }
    mWorkerPool.drain();
    mStopFlag.store(true, std::memory_order_release);
  }

  IpAddress mHostAddress;
  IpAddress mApiAddress;
  ThreadPool mWorkerPool{4};
  std::thread mTokenThread;
  std::atomic<int64_t> mTokenBucket{0};
  std::atomic<bool> mStopFlag{false};
};
} // namespace cpplay

int main() {
  cpplay::RateLimiter limiter{{{127, 0, 0, 1}, 4321}, {{127, 0, 0, 1}, 8081}};
  limiter.run();
}
