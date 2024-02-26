#include <gtest/gtest.h>

#include <exception>
#include <vector>

using namespace std;

class Solution {
  bool search1D(const vector<int> &row, const int target) {
    int l = 0, r = row.size() - 1;
    while (l <= r) {
      // l .. tl .. m .. tr .. r
      const int m = (l + r) / 2;
      const int val = row[m];
      if (target < val) // tl
        r = m - 1;
      else if (val < target) // tr
        l = m + 1;
      else
        return true;
    }
    return false;
  }

  bool search2D(const vector<vector<int>> &matrix, const int target) {
    int t = 0, b = matrix.size() - 1;
    while (t <= b) {
      // t .. tt .. front..back .. tb .. b
      const int m = (t + b) / 2;
      const vector<int> &row = matrix[m];
      if (target < row.front()) // tt
        b = m - 1;
      else if (row.back() < target) // tb
        t = m + 1;
      else
        return search1D(row, target);
    }
    return false;
  }

public:
  bool searchMatrix(const vector<vector<int>> &matrix, const int target) {
    return search2D(matrix, target);
  }
};

TEST(Search2DMatrixTest, LeetCodeExamplesExtened) {
  vector<vector<int>> matrix{
      {1, 3, 5, 7},
      {10, 11, 16, 20},
      {23, 30, 34, 60},
  };
  Solution s{};
  EXPECT_FALSE(s.searchMatrix(matrix, -100));
  EXPECT_TRUE(s.searchMatrix(matrix, 1));
  EXPECT_TRUE(s.searchMatrix(matrix, 3));
  EXPECT_TRUE(s.searchMatrix(matrix, 16));
  EXPECT_TRUE(s.searchMatrix(matrix, 34));
  EXPECT_TRUE(s.searchMatrix(matrix, 60));
  EXPECT_FALSE(s.searchMatrix(matrix, 100));
}

TEST(Search2DMatrixTest, LeetCodeExamplesReshaped1byN) {
  vector<vector<int>> matrix{
      {1, 3, 5, 7, 10, 11, 16, 20, 23, 30, 34, 60},
  };
  Solution s{};
  EXPECT_FALSE(s.searchMatrix(matrix, -100));
  EXPECT_TRUE(s.searchMatrix(matrix, 1));
  EXPECT_TRUE(s.searchMatrix(matrix, 3));
  EXPECT_TRUE(s.searchMatrix(matrix, 16));
  EXPECT_TRUE(s.searchMatrix(matrix, 34));
  EXPECT_TRUE(s.searchMatrix(matrix, 60));
  EXPECT_FALSE(s.searchMatrix(matrix, 100));
}

TEST(Search2DMatrixTest, LeetCodeExamplesReshapedNby1) {
  vector<vector<int>> matrix{
      {1}, {3}, {5}, {7}, {10}, {11}, {16}, {20}, {23}, {30}, {34}, {60},
  };
  Solution s{};
  EXPECT_FALSE(s.searchMatrix(matrix, -100));
  EXPECT_TRUE(s.searchMatrix(matrix, 1));
  EXPECT_TRUE(s.searchMatrix(matrix, 3));
  EXPECT_TRUE(s.searchMatrix(matrix, 16));
  EXPECT_TRUE(s.searchMatrix(matrix, 34));
  EXPECT_TRUE(s.searchMatrix(matrix, 60));
  EXPECT_FALSE(s.searchMatrix(matrix, 100));
}