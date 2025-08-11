#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

namespace cpplay {
struct ThreadPool {
  ThreadPool(size_t numThreads = 4, std::string alias = "WorkerPool");
  ~ThreadPool();

  void enqueue(std::function<void()> task);

private:
  void workerLoop();

  std::vector<std::thread> mWorkers;
  std::queue<std::function<void()>> mTasks;
  std::mutex mMutex;
  std::condition_variable mCv;
  std::atomic<bool> mStopFlag{false};

  std::string mAlias;
};

struct EventLoop {
  EventLoop();
  ~EventLoop();

  void run();
  void postTask(std::function<void()> task);
  void stop();

private:
  std::unique_ptr<ThreadPool> mPool;
};

struct Context : std::enable_shared_from_this<Context> {
  EventLoop mLoop;
  ThreadPool mPool;
};
} // namespace cpplay