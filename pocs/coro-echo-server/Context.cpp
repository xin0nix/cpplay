#include "Context.hpp"
#include <iostream> // Added for std::cerr

namespace cpplay {

ThreadPool::ThreadPool(size_t numThreads, std::string alias) : mAlias(alias) {
  std::cerr << mAlias + ": Starting with " << numThreads << " threads\n";
  for (size_t idx = 0UL; idx < numThreads; ++idx) {
    mWorkers.emplace_back(&ThreadPool::workerLoop, this);
    std::cerr << mAlias + ": Worker thread " << idx << " started\n";
  }
}

ThreadPool::~ThreadPool() {
  std::cerr << mAlias + ": Stopping all threads\n";
  mStopFlag.store(true);
  mCv.notify_all();
  for (auto &th : mWorkers) {
    if (th.joinable()) {
      th.join();
      std::cerr << mAlias + ": Worker thread joined\n";
    }
  }
  std::cerr << mAlias + ": All threads stopped\n";
}

void ThreadPool::enqueue(std::function<void()> task) {
  {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mStopFlag.load()) {
      std::cerr << mAlias + ": enqueue called after stop, throwing\n";
      throw std::runtime_error("Enqueue on stopped ThreadPool");
    }
    mTasks.push(std::move(task));
    std::cerr << mAlias + ": Task enqueued, total tasks: " << mTasks.size()
              << "\n";
  }
  mCv.notify_one();
  std::cerr << mAlias + ": Notified one worker\n";
}

void ThreadPool::workerLoop() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock lock(mMutex);
      std::cerr << mAlias + ": Worker waiting for task\n";
      mCv.wait(lock, [this]() { return !mTasks.empty() or mStopFlag.load(); });
      if (mStopFlag.load() and mTasks.empty()) {
        std::cerr << mAlias
                  << "Worker stopping due to stopFlag and empty "
                     "task queue\n";
        return;
      }
      task = std::move(mTasks.front());
      mTasks.pop();
      std::cerr << mAlias
                << "Worker picked a task, remaining tasks: " << mTasks.size()
                << "\n";
    }
    task();
    std::cerr << mAlias + ": Worker completed a task\n";
  }
}

EventLoop::EventLoop() : mPool(std::make_unique<ThreadPool>(1, "EventLoop")) {
  std::cerr << "EventLoop: Starting event loop\n";
}

EventLoop::~EventLoop() { std::cerr << "EventLoop: Stopping event loop\n"; }

void EventLoop::postTask(std::function<void()> task) {
  if (!mPool) {
    std::cerr << "EventLoop: postTask called but event loop not running\n";
    throw std::runtime_error("EventLoop is not running");
  }
  std::cerr << "EventLoop: Posting a new task\n";
  mPool->enqueue(std::move(task));
}

} // namespace cpplay
