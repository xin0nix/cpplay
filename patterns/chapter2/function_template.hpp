#pragma once

#include <iostream>

template <typename T> T increment(T val) { return val + 1; }

inline void function_template() { std::cout << increment(28) << std::endl; }

template <typename T> T sum(T from, T to, T step) {
  T res = from;
  while ((from += step) < to) {
    res += from;
  }
  return res;
}
