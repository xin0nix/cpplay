#include "solution.hpp"

#include <iostream>

int main() {
  std::vector<int> numbers{2, 2, 0, 1, 1, 0};
  // int n;
  // while (std::cin >> n) {
  //   numbers.push_back(n);
  // }
  // for (int n : numbers)
  //   std::cout << n << ", ";
  // std::cout << std::endl;
  int result = Solution().jump(numbers);
  std::cout << "Num of jumps: " << result << std::endl;
  return 0;
}
