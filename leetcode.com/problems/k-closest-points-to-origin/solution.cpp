#include "solution.hpp"

#include <algorithm>
#include <deque>

// Leetcode score:
// Final leet scope: Accepted
// Runtime: 290ms Beats 47.72% of users with C++
// Memory: 57.02mb Beats 95.44% of users with C++
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
