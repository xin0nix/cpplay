#include <gtest/gtest.h>

namespace detail {
template <class T> void swap(T &a, T &b) {
  T temp(a);
  a = b;
  b = temp;
}
} // namespace detail

// TODO:
