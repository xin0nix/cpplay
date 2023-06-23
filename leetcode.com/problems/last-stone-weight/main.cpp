#include "solution.hpp"

#include <iostream>

int main() {
  Solution solution;
  std::vector<int> stones;
  int s;
  while (std::cin >> s)
    stones.push_back(s);
  int lastStone = solution.lastStoneWeight(stones);
  std::cout << "The last stone: " << lastStone << std::endl;
  exit(EXIT_SUCCESS);
}
