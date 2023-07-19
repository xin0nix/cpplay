#include "solution.hpp"

#include <algorithm>
#include <deque>

// Leetcode score:
// Final leet scope: Accepted
// Runtime: 344ms Beats 39.02 % of users with C++
// Memory: 60.12mb Beats 92.10 % of users with C++
std::vector<std::vector<int>>
MinHeapSolution::kClosest(std::vector<std::vector<int>> &points, int k) {
  struct Point {
    int idx;
    unsigned long distance;
    Point() = delete;
    Point(const Point &) = delete;
    Point(Point &&) = default;
    Point &operator=(Point &&) = default;
    Point &operator=(const Point &) = delete;
    Point(int idx, const std::vector<int> &xy)
        : idx(idx), distance(xy[0] * xy[0] + xy[1] * xy[1]) {}
    bool operator<(const Point &other) const {
      // it's > because of the min heap
      return distance > other.distance;
    }
  };
  std::deque<Point> heap;
  for (int i = 0; i < points.size(); ++i)
    heap.emplace_back(i, points[i]);
  std::make_heap(heap.begin(), heap.end());
  std::vector<std::vector<int>> result;
  result.reserve(k);
  for (int i = 0; i < k; ++i) {
    std::pop_heap(heap.begin(), heap.end());
    result.push_back(points[heap.back().idx]);
    heap.pop_back();
  }
  return result;
}

// Leetcode score:
// Runtime: 197ms, Beats 96.29 % of users with C++
// Memory Details: 57.15mb, Beats 94.32 % of users with C++
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
