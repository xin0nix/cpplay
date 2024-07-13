#include <gtest/gtest.h>

namespace case_a {
std::string called;
// function template #1
template <class T> void f(T) { called = "void f(T t)"; }
// specialization of #1
template <> void f<int *>(int *) { called = "void f<int *>(int*)"; }
// function template #2
template <class T> void f(T *) { called = "void f(T*)"; }
} // namespace case_a

TEST(FuncTemplateSpecialization, CaseA) {
  case_a::called = "";
  int *p;
  case_a::f(p);
  ASSERT_EQ(case_a::called, "void f(T*)");
}

namespace case_b {
std::string called;
// function template #1
template <class T> void f(T) { called = "void f(T t)"; }
// overload
void f(int *) { called = "void f(int*)"; }
// function template #2
template <class T> void f(T *) { called = "void f(T*)"; }
} // namespace case_b

TEST(FuncTemplateSpecialization, CaseB) {
  case_b::called = "";
  int *p;
  case_b::f(p);
  ASSERT_EQ(case_b::called, "void f(int*)");
}

namespace odd_case {
constexpr int calc_sum(int a, int b) { return a + b; }
template <int (*Func)(int, int)> int call_bin_op(int a, int b) {
  return Func(a, b);
}
} // namespace odd_case

TEST(FuncTemplateSpecialization, OddCase) {
  ASSERT_EQ(odd_case::calc_sum(42, 28), 42 + 28);
}

namespace odd_case_b {
std::string picked = "";
template <class T, class P, int I> struct A {
  A() { picked = "A"; }
};

// Less strict specialization - T and P may be different
template <class T, class P, int I> struct A<T, P *, I> {
  A() { picked = "A<T, P *, I>"; }
};

// More strict specialization - we have T and pointer to T
template <class T, int I> struct A<T, T *, I> {
  A() { picked = "A<T, T *, I>"; }
};

template <class T> struct A<T, T *, 101> {
  A() { picked = "A<T, T *, 101>"; }
};

// Leads to ambiguous partial specialization
// template <class T, int I> class A<int, T *, I> {
//   A() { picked = "A<int, T *, I>"; }
// };
} // namespace odd_case_b

TEST(FuncTemplateSpecialization, OddCaseB) {
  odd_case_b::A<int, int *, 42> a;
  ASSERT_EQ(odd_case_b::picked, "A<T, T *, I>");
  odd_case_b::A<int, int *, 101> b;
  ASSERT_EQ(odd_case_b::picked, "A<T, T *, 101>");
  odd_case_b::A<int, float *, 101> c;
  ASSERT_EQ(odd_case_b::picked, "A<T, P *, I>");
}
