#include <coroutine>

#include <gtest/gtest.h>

namespace example_1 {
std::string global;
struct resumable {
  struct promise_type {
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() { return coro_handle::from_promise(*this); }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };
  using coro_handle = std::coroutine_handle<promise_type>;
  resumable(coro_handle handle) : handle_(handle) {}
  bool resume() {
    if (!handle_.done())
      handle_.resume();
    return !handle_.done();
  }
  resumable(resumable &&) = default;
  resumable(const resumable &) = delete;
  ~resumable() { handle_.destroy(); }

private:
  coro_handle handle_;
};

resumable foo() {
  global += "Hello";
  co_await std::suspend_always();
  global += "World";
}
} // namespace example_1

TEST(CoroutineTestSuite, Resumable) {
  auto t = example_1::foo();
  t.resume();
  example_1::global += " CoRoutine ";
  t.resume();
  ASSERT_EQ(example_1::global, "Hello CoRoutine World");
}
