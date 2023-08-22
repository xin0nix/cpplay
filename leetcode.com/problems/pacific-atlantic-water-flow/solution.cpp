#include "solution.hpp"

#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <set>

std::vector<std::vector<int>>
DepthFirstSolution::pacificAtlantic(std::vector<std::vector<int>> &heights) {
  using Point = std::pair<int, int>;
  using Set = std::set<Point>;
  const int ROWS = heights.size();
  const int COLS = heights[0].size();
  struct {
    const int ROWS;
    const int COLS;
    const std::vector<std::vector<int>> &heights;
    Set seen;
    void tryReach(int r, int c, int prevHeight) {
      if (seen.count({r, c}))
        return;
      if (r >= 0 && c >= 0 && r < ROWS && c < COLS &&
          heights[r][c] >= prevHeight)
        visit(r, c);
    };
    void visit(int r, int c) {
      seen.emplace(r, c);
      int h = heights[r][c];
      tryReach(r, c + 1, h);
      tryReach(r - 1, c, h);
      tryReach(r, c - 1, h);
      tryReach(r + 1, c, h);
    };
  } topLeft{ROWS, COLS, heights}, botRight{ROWS, COLS, heights};
  for (int i = 0; i < ROWS; ++i) {
    topLeft.visit(i, 0);
    botRight.visit(i, COLS - 1);
  }
  for (int j = 0; j < COLS; ++j) {
    topLeft.visit(0, j);
    botRight.visit(ROWS - 1, j);
  }
  std::vector<Point> intersection;
  std::set_intersection(topLeft.seen.begin(), topLeft.seen.end(),
                        botRight.seen.begin(), botRight.seen.end(),
                        std::back_inserter(intersection));
  std::vector<std::vector<int>> results;
  std::transform(intersection.begin(), intersection.end(),
                 std::back_inserter(results),
                 [](const Point &p) -> std::vector<int> {
                   return {p.first, p.second};
                 });
  return results;
}

std::vector<std::vector<int>>
BreadthFirstSolution::pacificAtlantic(std::vector<std::vector<int>> &heights) {
  using Point = std::pair<int, int>;
  using Queue = std::deque<Point>;
  using Set = std::set<Point>;
  const int ROWS = heights.size();
  const int COLS = heights[0].size();
  struct {
    const int ROWS;
    const int COLS;
    const std::vector<std::vector<int>> &heights;
    Queue q;
    Set seen;
    std::vector<Point> reachable;
    void reach(int r, int c, int prevHeight) {
      if (seen.count({r, c}))
        return;
      if (r >= 0 && c >= 0 && r < ROWS && c < COLS &&
          heights[r][c] >= prevHeight)
        reachable.emplace_back(r, c);
    };
    void visit() {
      while (!q.empty()) {
        Point pt = q.front();
        q.pop_front();
        seen.insert(pt);
        int r = pt.first;
        int c = pt.second;
        int h = heights[r][c];
        reachable.clear();
        reach(r, c + 1, h);
        reach(r, c - 1, h);
        reach(r + 1, c, h);
        reach(r - 1, c, h);
        for (Point &rpt : reachable)
          q.push_back(rpt);
      }
    };
  } topLeft{ROWS, COLS, heights}, botRight{ROWS, COLS, heights};
  for (int i = 0; i < ROWS; ++i) {
    topLeft.q.emplace_back(i, 0);
    botRight.q.emplace_back(i, COLS - 1);
  }
  for (int j = 0; j < COLS; ++j) {
    topLeft.q.emplace_back(0, j);
    botRight.q.emplace_back(ROWS - 1, j);
  }
  topLeft.visit();
  botRight.visit();
  std::vector<Point> intersection;
  std::set_intersection(topLeft.seen.begin(), topLeft.seen.end(),
                        botRight.seen.begin(), botRight.seen.end(),
                        std::back_inserter(intersection));
  std::vector<std::vector<int>> results;
  std::transform(intersection.begin(), intersection.end(),
                 std::back_inserter(results),
                 [](const Point &p) -> std::vector<int> {
                   return {p.first, p.second};
                 });
  return results;
}

std::vector<std::vector<int>>
Solution::pacificAtlantic(std::vector<std::vector<int>> &heights) {
  return DepthFirstSolution().pacificAtlantic(heights);
}
