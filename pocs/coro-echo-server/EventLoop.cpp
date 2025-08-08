#include "EventLoop.hpp"
#include <memory>
#include <stdexcept>

namespace cpplay {

EventLoop::EventLoop() { run(); }

EventLoop::~EventLoop() { stop(); }

void EventLoop::run() {
  if (!mPool) { // prevent restarting if already running
    mPool = std::make_unique<ThreadPool>(1);
  }
}

void EventLoop::stop() {
  if (mPool) {
    mPool.reset();
  }
}

void EventLoop::postTask(std::function<void()> task) {
  if (!mPool) {
    throw std::runtime_error("EventLoop is not running");
  }
  mPool->enqueue(std::move(task));
}

} // namespace cpplay