#include <coroutine>
#include <gtest/gtest.h>
#include <iostream>

namespace {
struct promise;

struct resumable : std::coroutine_handle<promise> {
  using promise_type = ::promise;
};

struct promise {
  resumable get_return_object() { return {resumable::from_promise(*this)}; }
  std::suspend_always initial_suspend() noexcept { return {}; }
  std::suspend_always final_suspend() noexcept { return {}; }
  void return_void() {}
  void unhandled_exception() {}
};
} // namespace

resumable foo() {
  std::cout << "Hello" << std::endl;
  co_await std::suspend_always();
  std::cout << "World" << std::endl;
}

TEST(CoroutineTestSuite, Resumable) {
  auto t = foo();
  t.resume();
  std::cout << "Coroutine" << std::endl;
  t.resume();
}
