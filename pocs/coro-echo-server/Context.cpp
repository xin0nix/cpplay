#include "Context.hpp"

namespace cpplay {
ThreadPool::ThreadPool(size_t numThreads) {
  for (size_t idx = 0UL; idx < numThreads; ++idx) {
    mWorkers.emplace_back(&ThreadPool::workerLoop, this);
  }
}

ThreadPool::~ThreadPool() {
  mStopFlag.store(true);
  mCv.notify_all();
  for (auto &th : mWorkers) {
    th.join();
  }
}

void ThreadPool::enqueue(std::function<void()> task) {
  {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mStopFlag.load()) {
      throw std::runtime_error("Enque on stopped ThreadPool");
    }
    mTasks.push(std::move(task));
  }
  mCv.notify_one();
}

void ThreadPool::workerLoop() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock lock(mMutex);
      mCv.wait(lock, [this]() { return not mTasks.empty() or mStopFlag; });
      if (mStopFlag.load() and mTasks.empty()) {
        return;
      }
      task = std::move(mTasks.front());
      mTasks.pop();
    }
    task();
  }
}

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