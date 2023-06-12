#include "solution.hpp"

#include <cassert>
#include <iostream>
#include <limits>
#include <vector>

/*
There are given N ropes of different lengths, we need to connect these ropes
into one rope. The cost to connect two ropes is equal to sum of their lengths.
The task is to connect the ropes with minimum cost. Given N size array arr[]
contains the lengths of the ropes.
*/

using Int = long long;

struct MinHeap {
  MinHeap(const MinHeap &) = delete;
  MinHeap(MinHeap &&) = delete;
  MinHeap(Int input[], int inputSize) : arr(input, input + inputSize) {
    buildMinHeap();
  }

  int getLeftIdx(const int i) const { return 2 * i + 1; }
  int getRightIdx(const int i) const { return getLeftIdx(i) + 1; }
  int getParentIdx(const int i) const { return (i - 1) / 2; }
  Int getParent(const int i) const { return arr[getParentIdx(i)]; }

  /// @brief Heapify 0-indecied array "arr" at position "p"
  /// @param p - the root of a subtree that is being heapified
  /// We assume that both left and right children are min heaps if present
  void minHeapify(int p) {
    int n = arr.size();
    assert(p >= 0 && p < n);
    // Be default p-th element of arr is considered to be the smallest one
    int smallest = p;
    while (true) {
      // Calculate left and right child indicies
      int left = getLeftIdx(p);
      int right = getRightIdx(p);
      // No children means nothing to heapify to begin with
      if (left >= n)
        break;
      // At this point we know for sure that at least the left child is present
      if (arr[left] < arr[smallest])
        smallest = left;
      // The right child is next to the left, and it might or might not exist
      if (right < n && arr[right] < arr[smallest])
        smallest = right;
      // The end of the loop
      if (smallest == p)
        break;
      // We figured out an index of the smallest element and if it is different
      // from p we should swap them and proceed
      std::swap(arr[p], arr[smallest]);
      // Go to the next iteration
      p = smallest;
    }
  }

  /// @brief Build min heap out of an unsorted array "arr"
  void buildMinHeap() {
    if (arr.empty())
      return;
    // Given that `parent(i) = floor((i - 1) / 2)`
    // And the number of elements is n (and i < n).
    // The last non-leaf node starts at `parent(n-1)`
    int p = getParentIdx(arr.size() - 1);
    // Now we can traverse elements of the "arr" to top-down way, starting from
    // the last parent node "p" and down to the root, indecied with 0.
    while (p >= 0)
      minHeapify(p--);
  }

  /// @brief Returns the smallest element of the min-heap
  /// @return
  Int minElement() const {
    assert(!arr.empty());
    return arr[0];
  }

  /// @brief Extracts and returns the minimum element of the heap
  /// @return
  Int extractMin() {
    assert(!arr.empty());
    Int min = arr[0];
    std::swap(arr[0], arr[arr.size() - 1]);
    arr.pop_back();
    if (arr.size())
      minHeapify(0);
    return min;
  }

  /// @brief Decreas value in the min heap
  /// @param i - index of the value
  /// @param newVal - new value, that must be less or equal to arr[i]
  void decrease(Int i, Int newVal) {
    assert(i <= arr.size());
    assert(arr[i] >= newVal);
    arr[i] = newVal;
    int p = getParentIdx(i);
    // We should stop it 2 cases:
    // - the parent is less or equal to its child
    // - the heap undeflows, i.e. i goes equal 0 (thus p < 0)
    while (i > 0 && arr[p] > arr[i]) {
      std::swap(arr[p], arr[i]);
      i = p;
      p = getParentIdx(p);
    }
  }

  /// @brief Insert value into the min heap
  /// @param val - new value
  void insert(Int val) {
    // We insert the maximum value possible to ensure that the following
    // property holds: arr[i] >= newVal
    arr.push_back(std::numeric_limits<Int>::max());
    decrease(arr.size() - 1, val);
  }

  int size() const { return arr.size(); }
  bool empty() const { return arr.empty(); }
  void dump() const {
    for (Int a : arr)
      std::cout << a << ", ";
    std::cout << std::endl;
  }

private:
  /// @brief Array that represents elements of the heap
  std::vector<Int> arr;
};

Int Solution::minCost(Int inputArray[], Int inputArraySize) {
  if (inputArraySize < 2)
    return 0;
  MinHeap minHeap(inputArray, inputArraySize);
  Int totalCost = 0;
  while (minHeap.size() > 1) {
    Int shortest = minHeap.extractMin();
    Int nextShortest = minHeap.extractMin();
    Int newPiece = shortest + nextShortest;
    totalCost += newPiece;
    minHeap.insert(newPiece);
  }
  return totalCost;
}
