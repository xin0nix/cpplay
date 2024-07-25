#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <type_traits>
#include <vector>

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

struct Object {
  static std::vector<int> ctors;
  static std::vector<int> dtors;
  Object(int id) : id(id) { ctors.push_back(id); }
  ~Object() { dtors.push_back(id); }
  int id;
};

std::vector<int> Object::ctors;
std::vector<int> Object::dtors;

void func1() {
  Object obj1(1);
  Object obj2(2);
  throw std::exception();
}

void func2() {
  Object obj3(3);
  func1();
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

TEST(Chapter11Test, OffTop1) {
  try {
    detail::func2();
  } catch (...) {
  }
  EXPECT_THAT(detail::Object::ctors, ::testing::ElementsAre(3, 1, 2));
  EXPECT_THAT(detail::Object::dtors, ::testing::ElementsAre(2, 1, 3));
}
