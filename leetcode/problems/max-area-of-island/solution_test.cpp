#include <gtest/gtest.h>

#include <deque>
#include <vector>

using namespace std;

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

      void countAndForget(pair<int, int> point) {
        auto [row, col] = point;
        grid[row][col] = 0;
        counter += 1;
      }

      auto operator()(pair<int, int> point) {
        // we'll modify the grid to mark visited cells
        deque<pair<int, int>> candidates{point};
        while (not candidates.empty()) {
          auto cur = candidates.front();
          candidates.pop_front();
          if (!isIsland(cur)) {
            continue;
          }
          countAndForget(cur);
          auto [row, col] = cur;
          candidates.push_back({row, col + 1});
          candidates.push_back({row, col - 1});
          candidates.push_back({row + 1, col});
          candidates.push_back({row - 1, col});
        }
        return counter;
      }
      vector<vector<int>> &grid;
      int counter{0};
    };
    int maxRes = 0;
    for (int row = 0; row < grid.size(); ++row) {
      for (int col = 0; col < grid.front().size(); ++col) {
        int res = Bfs{grid}({row, col});
        maxRes = std::max(maxRes, res);
      }
    }
    return maxRes;
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
