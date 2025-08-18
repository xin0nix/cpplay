#include "IpAddress.hpp"
#include "TcpSocket.hpp"
#include "ThreadPool.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace {
std::atomic<bool> gTerminate(false);

void signalHandler(int sig) {
  gTerminate.store(true, std::memory_order_release);
  ::signal(sig, SIG_DFL);
}
} // namespace

namespace cpplay {
struct RateLimiter {
  RateLimiter(IpAddress hostAddress, IpAddress apiAddress)
      : mHostAddress(hostAddress), mApiAddress(apiAddress) {}

  ~RateLimiter() {
    if (mTokenThread && mTokenThread->joinable()) {
      mTokenThread->join();
    }
  }

  void start() {
    mTokenThread =
        std::make_unique<std::thread>(&RateLimiter::tokenSupplier, this);
    mWorkerPool.start(4);
  }

  void tokenSupplier() {
    auto lastRefill = std::chrono::steady_clock::now();
    const int64_t maxTokens = 10;     // Burst capacity
    const auto refillInterval = 0.1s; // Add 1 token every 0.1 seconds

    while (!mStopFlag.load(std::memory_order_acquire)) {
      auto now = std::chrono::steady_clock::now();
      auto elapsed = now - lastRefill;
      int64_t tokensToAdd = elapsed / refillInterval;
      if (tokensToAdd > 0) {
        int64_t newTokens{0};
        auto current = mTokenBucket.load(std::memory_order_acquire);
        do {
          newTokens = std::min(current + tokensToAdd, maxTokens);
          // NOTE: compare_exchange_strong updates current, not need to reload
        } while (!mTokenBucket.compare_exchange_weak(
            current, newTokens,
            std::memory_order_acq_rel, // success ordering
            std::memory_order_acquire  // failure ordering
            ));
        lastRefill = now;
      }
      std::this_thread::sleep_for(100ms);
    }
  }

  bool tryGetToken() {
    int64_t current;
    do {
      current = mTokenBucket.load(std::memory_order_acquire);
      if (current <= 0) {
        return false;
      }
    } while (!mTokenBucket.compare_exchange_weak(
        current, current - 1,
        std::memory_order_acq_rel, // success ordering
        std::memory_order_acquire  // failure ordering
        ));
    return true;
  }

  void run(std::function<bool()> predicate) {
    // FIXME: infinite loop or something
    TcpSocket acceptor;
    acceptor.bindTo(mHostAddress);
    while (predicate()) {
      // Accept a connection
      try {
        auto connectionFd = acceptor.acceptConneciton();
        mWorkerPool.enqueue([this, connectionFd]() {
          TcpSocket client{connectionFd};
          if (!tryGetToken()) {
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
    mStopFlag.store(true, std::memory_order_release);
    mWorkerPool.drain();
  }

  IpAddress mHostAddress;
  IpAddress mApiAddress;
  ThreadPool mWorkerPool;
  std::unique_ptr<std::thread> mTokenThread;
  std::atomic<int64_t> mTokenBucket{0};
  std::atomic<bool> mStopFlag{false};
};
} // namespace cpplay

int main() {
  std::signal(SIGINT, signalHandler);  // Ctrl-C
  std::signal(SIGTERM, signalHandler); // kill command
  cpplay::RateLimiter limiter{{{127, 0, 0, 1}, 4321}, {{127, 0, 0, 1}, 8081}};
  limiter.start();
  limiter.run([]() { return !gTerminate; });
}
