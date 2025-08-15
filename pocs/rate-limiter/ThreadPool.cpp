#include "ThreadPool.hpp"
#include <iostream>

namespace cpplay {
ThreadPool::ThreadPool(size_t numThreads) {
  std::cerr << "ThreadPool: Starting with " << numThreads << " threads.\n";
  for (size_t i = 0UL; i < numThreads; ++i) {
    mThreads.emplace_back(&ThreadPool::workerLoop, this);
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock lock(mMutex);
    std::cerr << "ThreadPool: Stopping all threads.\n";
    mStopFlag = true;
  }
  mCv.notify_all();
  for (auto &th : mThreads) {
    if (th.joinable()) {
      th.join();
    }
  }
  std::cerr << "ThreadPool: All threads stopped.\n";
}

void ThreadPool::drain() {
  std::unique_lock lock(mMutex);
  mCv.wait(lock, [this]() { return mTasks.empty() && (mRunningTasks == 0); });
  std::cerr << "ThreadPool: drain() complete. All tasks finished.\n";
}

void ThreadPool::workerLoop() {
  while (true) {
    Task task;
    {
      std::unique_lock lock(mMutex);
      mCv.wait(lock, [this]() { return !mTasks.empty() || mStopFlag; });
      if (mStopFlag && mTasks.empty()) {
        std::cerr << "ThreadPool: Worker thread exiting.\n";
        return;
      }
      task = std::move(mTasks.front());
      mTasks.pop();
      ++mRunningTasks;
    }
    try {
      task();
    } catch (const std::exception &ex) {
      std::cerr << "Task failed with exception: " << ex.what() << "\n";
    } catch (...) {
      std::cerr << "Task failed with unknown exception.\n";
    }
    --mRunningTasks;
    mCv.notify_all();
  }
}

void ThreadPool::enqueue(Task task) {
  {
    std::unique_lock lock(mMutex);
    if (mStopFlag) {
      std::cerr << "ThreadPool enqueue called after stop flag set!\n";
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    mTasks.push(std::move(task));
    std::cerr << "ThreadPool: Task enqueued. Queue size is now "
              << mTasks.size() << ".\n";
  }
  mCv.notify_all();
}
} // namespace cpplay
