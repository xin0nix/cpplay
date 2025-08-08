#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <vector>

namespace cpplay {
struct ThreadPool {
  ThreadPool(size_t numThreads);
  ~ThreadPool();

  void enqueue(std::function<void()> task);
  void stop();

private:
  void workerLoop();

  std::vector<std::thread> mWorkers;
  std::queue<std::function<void()>> mTasks;
  std::mutex mMutex;
  std::condition_variable mCv;
  std::atomic<bool> mStopFlag{false};
};
} // namespace cpplay