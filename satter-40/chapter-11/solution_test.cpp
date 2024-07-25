#include <gtest/gtest.h>
#include <type_traits>

namespace detail {
template <typename T> void foo() {
  if constexpr (std::is_integral_v<T>)
    throw 42;
  if constexpr (std::is_same_v<T, std::string>)
    throw std::string("hello");
}
template <typename T> void bar() {
  try {
    foo<T>();
  } catch (const std::string &) {
  }
}
} // namespace detail

TEST(Chapter11Test, Case1) {
  ASSERT_THROW(detail::foo<int>(), int);
  ASSERT_THROW(detail::foo<std::string>(), std::string);
  ASSERT_NO_THROW(detail::foo<float>());

  ASSERT_THROW(detail::bar<int>(), int);
  ASSERT_NO_THROW(detail::bar<std::string>());
  ASSERT_NO_THROW(detail::bar<float>());
}
