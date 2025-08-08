#include "ThreadPool.hpp"
#include <mutex>
#include <stdexcept>

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
} // namespace cpplay