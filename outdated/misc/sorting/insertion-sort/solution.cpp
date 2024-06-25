#include "solution.hpp"

void Solution::sort(std::vector<int> &arr) {
  // At each iteration, all the elements on the left from idx are sorted
  for (int idx = 0; idx < arr.size(); ++idx) {
    int cur = arr[idx];
    int prev = idx - 1;
    // Find the element (if any) on the left that is larger than arr[idx]
    while (prev >= 0 && arr[prev] > cur) {
      // Shift prev to the right (bubble up if so-to-speak)
      arr[prev + 1] = arr[prev];
      // If arr[prev] is greater than cur, it will be replaced with cur
      --prev;
    }
    // If the "while" loop is skipped, prev+1 points to idx
    arr[prev + 1] = cur;
  }
}
