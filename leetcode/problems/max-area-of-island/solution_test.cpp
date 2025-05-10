#include <gtest/gtest.h>

#include <queue>
#include <vector>

using namespace std;

#include <ranges>
namespace rng = std::ranges;
namespace view = std::views;

struct Solution {

  int maxAreaOfIsland(vector<vector<int>> &grid) {
    struct Bfs {
      bool isIsland(pair<int, int> point) const {
        auto [row, col] = point;
        if (row < 0 or row >= grid.size()) {
          return false;
        }
        if (col < 0 or col >= grid.front().size()) {
          return false;
        }
        return 1 == grid[row][col];
      }

      void forget(pair<int, int> point) {
        auto [row, col] = point;
        grid[row][col] = 0;
      }

      auto operator()(int row = 0, int col = 0) {
        // we'll modify the grid to mark visited cells
        deque<pair<int, int>> candidates{{row, col}};
        while (not candidates.empty()) {
          auto cur = candidates.front();
          candidates.pop_front();
          if (!isIsland(cur)) {
            continue;
          }
          forget(cur);
        }
      }
      vector<vector<int>> &grid;
      // TODO: nxm exploration + bfs on each island
      int res{0};
    };
    return 28;
  }
};

struct TestCase {
  vector<vector<int>> grid;
  int expectedArea;
};

class MaxAreaOfIslandParamTest : public ::testing::TestWithParam<TestCase> {};

TEST_P(MaxAreaOfIslandParamTest, TestCases) {
  Solution sol;
  TestCase testCase = GetParam();
  EXPECT_EQ(sol.maxAreaOfIsland(testCase.grid), testCase.expectedArea);
}

INSTANTIATE_TEST_SUITE_P(
    MaxAreaOfIslandTests, MaxAreaOfIslandParamTest,
    ::testing::Values(TestCase{{
                                   {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                                   {0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                                   {0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0},
                                   {0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                               },
                               6},
                      TestCase{{
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                               },
                               0},
                      TestCase{{
                                   {1, 1, 1},
                                   {1, 1, 1},
                                   {1, 1, 1},
                               },
                               9},
                      TestCase{{
                                   {0, 0, 0},
                                   {0, 1, 0},
                                   {0, 0, 0},
                               },
                               1},
                      TestCase{{
                                   {1, 0, 0, 1},
                                   {0, 0, 1, 0},
                                   {1, 1, 0, 0},
                               },
                               2},
                      TestCase{{}, 0},
                      TestCase{{
                                   {1, 1, 0, 1, 1, 1},
                               },
                               3},
                      TestCase{{
                                   {1},
                                   {1},
                                   {0},
                                   {1},
                                   {1},
                               },
                               2}));
