#include <gtest/gtest.h>

#include <type_traits>

namespace detail {
template <class T, typename Enable = std::enable_if_t<
                       std::conjunction_v<std::is_nothrow_constructible<T>,
                                          std::is_nothrow_copy_assignable<T>>>>
void swap(T &a, T &b) {
  T temp(a);
  a = b;
  b = temp;
}
} // namespace detail

struct NonCopiable {
  NonCopiable(int) {}
  NonCopiable(const NonCopiable &) = delete;
  NonCopiable &operator=(const NonCopiable &) = delete;
};

struct Exceptional {
  Exceptional(int) {}
  Exceptional(const Exceptional &) noexcept(false) {
    throw std::logic_error("Fail!");
  }
  Exceptional &operator=(const Exceptional &) noexcept(false) {
    throw std::logic_error("Fail!");
    return *this;
  }
};

struct Ordinary {};

TEST(SwapTest, CaseA) {
  int a = 1;
  int b = 2;
  detail::swap(a, b);
  ASSERT_EQ(a, 2);
  ASSERT_EQ(b, 1);
}

// TEST(SwapTest, CaseNonCopiable) {
//   NonCopiable a = 1;
//   NonCopiable b = 2;
//   detail::swap(a, b);
// }

// TEST(SwapTest, CaseExceptional) {
//   Exceptional a(1);
//   Exceptional b(2);
//   detail::swap(a, b);
// }

TEST(SwapTest, CaseOrdinary) {
  Ordinary a;
  Ordinary b;
  detail::swap(a, b);
}
