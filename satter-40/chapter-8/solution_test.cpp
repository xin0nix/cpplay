#include <gtest/gtest.h>

#include <memory>

namespace detail {
enum class Called {
  Nothing,
  Destructible,
  Ordinary,
} called;
template <std::destructible T> void checked_delete(T *ptr) noexcept {
  if (ptr) {
    std::destroy_at(ptr);
    delete ptr;
    return;
  }
  called = Called::Nothing;
}
} // namespace detail

TEST(Chapter8Tests, DestructibleTest1) {
  using namespace detail;
  called = Called::Nothing;
  struct Cat {
    ~Cat() { called = Called::Destructible; }
  };
  auto *cat = new Cat{};
  checked_delete(cat);
  ASSERT_EQ(called, Called::Destructible);
  cat = nullptr;
  ASSERT_NO_FATAL_FAILURE(checked_delete(cat));
  ASSERT_EQ(called, Called::Nothing);
}
