#include <gtest/gtest.h>

#include <exception>
#include <vector>

using namespace std;

struct Solution {
  bool searchMatrix(const vector<vector<int>> &matrix, int target) {
    throw exception();
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