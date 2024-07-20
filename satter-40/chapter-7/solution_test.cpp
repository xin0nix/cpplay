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

namespace dimov_abrahams_case_a {
std::string called = "";
// (a) primary template
template <typename T> void f(T) { called = "a"; }
// (b) primary template, overloading (a)
template <typename T> void f(T *) { called = "b"; }
// (c) explicit specialization of (b)
template <> void f<int>(int *) { called = "c"; }
} // namespace dimov_abrahams_case_a

TEST(FuncTemplateSpecialization, DimovAbrahamsCaseA) {
  int *p;
  dimov_abrahams_case_a::f(p);
  ASSERT_EQ(dimov_abrahams_case_a::called, "c");
}

namespace dimov_abrahams_case_b {
std::string called = "";
// (a) primary template
template <typename T> void f(T) { called = "a"; }
// (c) explicit specialization of (a), not (b)!
template <> void f<int *>(int *) { called = "c"; }
// (b) primary template, overloading (a)
template <typename T> void f(T *) { called = "b"; }
} // namespace dimov_abrahams_case_b

TEST(FuncTemplateSpecialization, DimovAbrahamsCaseB) {
  int *p;
  dimov_abrahams_case_b::f(p);
  ASSERT_EQ(dimov_abrahams_case_b::called, "b");
}

namespace morale_2 {
std::string called = "";
template <class T> struct FImpl;
template <class T> void f(T t) { FImpl<T>::f(t); } // Do not touch
template <class T> struct FImpl {
  static void f(T t) { called = "generic"; }
};
template <> struct FImpl<int> {
  static void f(int) { called = "int"; }
};
template <> struct FImpl<double> {
  static void f(double) { called = "double"; }
};
} // namespace morale_2

TEST(FuncTemplateSpecialization, Morale2) {
  using namespace morale_2;
  int p;
  f(p);
  ASSERT_EQ(called, "int");
  double q;
  f(q);
  ASSERT_EQ(called, "double");
  float y;
  f(y);
  ASSERT_EQ(called, "generic");
  struct {
  } x;
  f(x);
  ASSERT_EQ(called, "generic");
}
