#include "solution.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <vector>

/*
There are given N ropes of different lengths, we need to connect these ropes
into one rope. The cost to connect two ropes is equal to sum of their lengths.
The task is to connect the ropes with minimum cost. Given N size array arr[]
contains the lengths of the ropes.
*/

using Int = long long;

struct PriorityQueueInterface {
  PriorityQueueInterface() = default;
  PriorityQueueInterface(const PriorityQueueInterface &) = delete;
  PriorityQueueInterface(PriorityQueueInterface &&) = delete;
  virtual Int extractMin() = 0;
  virtual void insert(Int val) = 0;
  virtual int size() const = 0;
  virtual void dump() const = 0;
};

struct StdMinHeap : PriorityQueueInterface {
  StdMinHeap(Int input[], int inputSize) : arr(input, input + inputSize) {
    std::make_heap(arr.begin(), arr.end(), std::greater<>{});
  }

  virtual Int extractMin() final {
    Int min = arr.front();
    std::pop_heap(arr.begin(), arr.end(), std::greater<>{});
    arr.pop_back();
    return min;
  }
  virtual void insert(Int val) final {
    arr.push_back(val);
    std::push_heap(arr.begin(), arr.end(), std::greater<>{});
  }
  virtual int size() const final { return arr.size(); }
  virtual void dump() const final {
    for (Int a : arr)
      std::cout << a << ", ";
    std::cout << std::endl;
  }

  std::vector<Int> arr;
};

struct MapQueue : PriorityQueueInterface {
  int totalSize = 0;
  MapQueue(Int input[], int inputSize) {
    for (int i = 0; i < inputSize; ++i)
      insert(input[i]);
  }

  virtual Int extractMin() final {
    assert(pqMap.size());
    auto minEl = pqMap.begin();
    Int minVal = minEl->first;
    minEl->second--;
    if (minEl->second == 0)
      pqMap.erase(minEl);
    --totalSize;
    return minVal;
  }

  virtual void insert(Int val) final {
    pqMap[val]++;
    ++totalSize;
  }

  virtual int size() const final {
    assert(totalSize >= pqMap.size());
    return totalSize;
  }

  virtual void dump() const final {
    if (size() == 0)
      std::cout << "empty\n";
    for (auto [val, count] : pqMap)
      for (int i = 0; i < count; ++i)
        std::cout << val << ", ";
    std::cout << "\n";
  }

private:
  std::map<Int, int> pqMap;
};

struct MinHeap : PriorityQueueInterface {
  MinHeap(Int input[], int inputSize) : arr(input, input + inputSize) {
    std::make_heap(arr.begin(), arr.end());
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
  virtual Int extractMin() final {
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
  virtual void insert(Int val) final {
    // We insert the maximum value possible to ensure that the following
    // property holds: arr[i] >= newVal
    arr.push_back(std::numeric_limits<Int>::max());
    decrease(arr.size() - 1, val);
  }

  virtual int size() const final { return arr.size(); }
  bool empty() const { return arr.empty(); }
  virtual void dump() const final {
    for (Int a : arr)
      std::cout << a << ", ";
    std::cout << std::endl;
  }

private:
  /// @brief Array that represents elements of the heap
  std::vector<Int> arr;
};

// Implementation notes
// Performance on geeks4geeks:
// - min heap takes 4.16 to finish
// - map-based takes 1.8 (x2 boost!)
Int Solution::minCost(Int inputArray[], Int inputArraySize, SolutionKind sk) {
  Int *arrClone = new Int[inputArraySize];
  for (int i = 0; i < inputArraySize; ++i)
    arrClone[i] = inputArray[i];
  if (inputArraySize < 2)
    return 0;
  std::unique_ptr<PriorityQueueInterface> priorityQueue = nullptr;
  switch (sk) {
  case SolutionKind::SK_MinHeap:
    priorityQueue = std::make_unique<MinHeap>(arrClone, inputArraySize);
    break;
  case SolutionKind::SK_MapPQ:
    priorityQueue = std::make_unique<MapQueue>(arrClone, inputArraySize);
    break;
  case SolutionKind::SK_STDMinHeap:
    priorityQueue = std::make_unique<StdMinHeap>(arrClone, inputArraySize);
    break;
  default:
    abort();
  }
  Int totalCost = 0;
  while (priorityQueue->size() > 1) {
    Int shortest = priorityQueue->extractMin();
    Int nextShortest = priorityQueue->extractMin();
    Int newPiece = shortest + nextShortest;
    totalCost += newPiece;
    priorityQueue->insert(newPiece);
  }
  delete[](arrClone);
  return totalCost;
}
