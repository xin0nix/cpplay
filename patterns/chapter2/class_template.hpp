#pragma once

#include <cstddef>
#include <iostream>

template <typename T> class ArrayOf2 {
public:
  T &operator[](size_t i) { return a_[i]; }
  const T &operator[](size_t i) const { return a_[i]; }
  T sum() const { return a_[0] + a_[1]; }

private:
  T a_[2];
};

void inline class_template() {
  ArrayOf2<double> a;
  a[0] = 1;
  a[1] = 2;
  std::cout << a.sum() << std::endl;
}