#include "solution.hpp"

#include <algorithm>
#include <deque>
#include <set>
#include <unordered_set>

int RewriterSolution::numIslands(std::vector<std::vector<char>> &grid) {
  if (grid.empty())
    return 0;
  const size_t ROWS = grid.size();
  const size_t COLS = grid[0].size();

  struct {
    std::vector<std::vector<char>> &grid;
    const size_t ROWS;
    const size_t COLS;
    void visit(size_t i, size_t j) {
      if (i < 0 || j < 0 || i >= ROWS || j >= COLS)
        return;
      if (grid[i][j] != '1')
        return;
      grid[i][j] = '#'; // visited
      visit(i, j + 1);
      visit(i, j - 1);
      visit(i - 1, j);
      visit(i + 1, j);
    };
  } visitor{grid, ROWS, COLS};

  int counter = 0;
  for (int i = 0; i < ROWS; ++i) {
    for (int j = 0; j < COLS; ++j) {
      if (grid[i][j] != '1')
        continue;
      visitor.visit(i, j);
      ++counter;
    }
  }

  return counter;
}

int SetNDequeSolution::numIslands(std::vector<std::vector<char>> &grid) {
  if (grid.empty())
    return 0;
  const size_t rows = grid.size();
  const size_t cols = grid[0].size();

  // coordinates of the points we've already visited
  std::set<std::pair<int, int>> seen;
  std::deque<std::pair<int, int>> queue;
  int counter = 0;

  auto handle = [&](std::pair<int, int> &&pt) {
    if (pt.first < 0 || pt.second < 0)
      return;
    if (pt.first >= rows || pt.second >= cols)
      return;
    if (grid[pt.first][pt.second] == '0')
      return;
    if (seen.count(pt))
      return;
    queue.push_back(pt);
    seen.insert(std::move(pt));
  };

  auto bfs = [&](std::pair<int, int> &&coordinates) {
    // we have a set of the nodes we've already 'seen'
    queue.push_back(coordinates);
    seen.insert(std::move(coordinates));
    while (!queue.empty()) {
      std::pair<int, int> pt = queue.front();
      queue.pop_front();
      // up/down
      handle({pt.first - 1, pt.second});
      handle({pt.first + 1, pt.second});
      // left/right
      handle({pt.first, pt.second - 1});
      handle({pt.first, pt.second + 1});
    }
  };

  // traverse the grid
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      std::pair<int, int> coordanates{i, j};
      // skip both 'water' and visited islands
      if (grid[i][j] == '0' || seen.count(coordanates))
        continue;
      bfs(std::move(coordanates));
      ++counter;
    }
  }
  return counter;
}

int Solution::numIslands(std::vector<std::vector<char>> &grid) {
  return RewriterSolution().numIslands(grid);
}
