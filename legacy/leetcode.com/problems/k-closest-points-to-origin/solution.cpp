#include "solution.hpp"

#include <algorithm>
#include <deque>

std::vector<std::vector<int>>
MinHeapSolution::kClosest(std::vector<std::vector<int>> &points, int k) {
  auto distance = [](const vector<int> &xy) -> unsigned long {
    return xy[0] * xy[0] + xy[1] * xy[1];
  };
  auto cmp = [&distance](const auto &a, const auto &b) -> bool {
    return distance(a) > distance(b); // opposite to default max-heap
  };
  std::make_heap(points.begin(), points.end(), cmp);
  std::vector<std::vector<int>> result;
  result.reserve(k);
  for (int i = 0; i < k; ++i) {
    result.push_back(points.front());
    std::pop_heap(points.begin(), points.end(), cmp);
    points.pop_back();
  }
  return result;
}

std::vector<std::vector<int>>
MaxHeapSolution::kClosest(std::vector<std::vector<int>> &points, int k) {
  auto distance = [](const vector<int> &xy) -> unsigned long {
    return xy[0] * xy[0] + xy[1] * xy[1];
  };
  auto cmp = [&distance](const auto &a, const auto &b) -> bool {
    return distance(a) < distance(b); // it's a max-heap, cmp = lt
  };
  std::deque<std::vector<int>> heap;
  for (auto &pt : points) {
    heap.push_back(std::move(pt)); // ok?
    std::push_heap(heap.begin(), heap.end(), cmp);
    if (heap.size() > (size_t)k) {
      std::pop_heap(heap.begin(), heap.end(), cmp);
      heap.pop_back();
    }
  }
  std::vector<std::vector<int>> result;
  std::copy(heap.begin(), heap.end(), std::back_inserter(result));
  return result;
}

std::vector<std::vector<int>>
NthElementSolution::kClosest(std::vector<std::vector<int>> &points, int k) {
  auto distance = [](const vector<int> &xy) -> unsigned long {
    return xy[0] * xy[0] + xy[1] * xy[1];
  };
  auto nth = points.begin();
  advance(nth, k);
  nth_element(points.begin(), nth, points.end(),
              [&distance](const auto &a, const auto &b) -> bool {
                return distance(a) < distance(b);
              });
  points.resize(k);
  return points;
}

std::vector<std::vector<int>>
Solution::kClosest(std::vector<std::vector<int>> &points, int k) {
  return MinHeapSolution().kClosest(points, k);
}
