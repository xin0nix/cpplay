#include <gtest/gtest.h>

#include <tuple>
#include <vector>

class Solution {
public:
  int uniquePaths(int numRows, int numCols) {
    std::vector<std::vector<int>> grid;
    grid.resize(numRows + 1);
    for (auto &row : grid) {
      row.resize(numCols + 1);
    }
    grid.at(numRows - 1).at(numCols) = 1;
    for (int row = numRows - 1; row >= 0; --row) {
      for (int col = numCols - 1; col >= 0; --col) {
        int right = grid.at(row).at(col + 1);
        int bottom = grid.at(row + 1).at(col);
        grid.at(row).at(col) = right + bottom;
      }
    }
    return grid.at(0).at(0);
  }
};

class UniquePathsTest
    : public testing::TestWithParam<std::tuple<int, int, int>> {
protected:
  Solution solution;
};

TEST_P(UniquePathsTest, ReturnsCorrectPathCount) {
  auto [m, n, expected] = GetParam();
  EXPECT_EQ(solution.uniquePaths(m, n), expected);
}

INSTANTIATE_TEST_SUITE_P(UniquePathsSuite, UniquePathsTest,
                         testing::Values(
                             // Edge cases
                             std::make_tuple(1, 1, 1), std::make_tuple(1, 5, 1),
                             std::make_tuple(5, 1, 1),

                             // Standard cases
                             std::make_tuple(3, 2, 3),
                             std::make_tuple(3, 7, 28),
                             std::make_tuple(2, 2, 2),

                             // Symmetric case
                             std::make_tuple(7, 3, 28)));
