#include "solution.hpp"

/*
see: https://leetcode.com/problems/last-stone-weight/

You are given an array of integers stones where stones[i] is the weight of the
ith stone.

We are playing a game with the stones. On each turn, we choose the heaviest two
stones and smash them together. Suppose the heaviest two stones have weights x
and y with x <= y. The result of this smash is:

- If x == y, both stones are destroyed, and
- If x != y, the stone of weight x is destroyed, and the stone of weight y has
new weight y - x. At the end of the game, there is at most one stone left.

Return the weight of the last remaining stone. If there are no stones left,
return 0.
*/

#include <algorithm>

int Solution::lastStoneWeight(vector<int> &stones) {
  // 1. Make a heap
  // 2. Until we have at least 2 stones, pick the largest ones, substract, and
  // if the result is not 0, put it back
  // 3. If the stones array is empty, return 0, otherwise - return the
  // remaining stone
  std::make_heap(stones.begin(), stones.end());
  auto pick_heaviest_stone = [&stones]() -> int {
    int h = stones.front();
    std::pop_heap(stones.begin(), stones.end());
    stones.pop_back();
    return h;
  };
  while (stones.size() > 1) {
    int a = pick_heaviest_stone();
    int b = pick_heaviest_stone();
    int c = a - b; // a >= b
    if (c == 0)
      continue; // skip this stone, as it doesn't exist
    stones.push_back(c);
    std::push_heap(stones.begin(), stones.end());
  }
  if (stones.empty())
    return 0;
  return stones.front();
}
