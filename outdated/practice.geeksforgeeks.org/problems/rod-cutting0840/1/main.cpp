#include "solution.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>

int main() {
  const int n = 10;
  int *price = new int[n]();
  std::iota(price, price + n, 1);
  std::cout << "Prices:\n";
  for (int i = 0; i < n; ++i)
    std::cout << price[i] << ", ";
  std::cout << "\n";
  std::cout << "Max profit: " << Solution().cutRod(price, n);
  delete[] price;
  return 0;
}
