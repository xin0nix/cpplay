#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace cpplay {
using Task = std::function<void(void)>;

struct ThreadPool final {

  ThreadPool(size_t numThreads);
  ~ThreadPool();

  void enqueue(Task task);
  void drain();

private:
  void workerLoop();

  bool mStopFlag{false};
  std::mutex mMutex;
  std::condition_variable mCv;
  std::queue<Task> mTasks;
  std::vector<std::thread> mThreads;
  std::atomic<size_t> mRunningTasks;
};
} // namespace cpplay
