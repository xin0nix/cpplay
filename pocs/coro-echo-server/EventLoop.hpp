#pragma once

#include <functional>

#include "ThreadPool.hpp"

namespace cpplay {
struct EventLoop {
  EventLoop();
  ~EventLoop();

  void run();
  void postTask(std::function<void()> task);
  void stop();

private:
  std::unique_ptr<ThreadPool> mPool;
};
} // namespace cpplay