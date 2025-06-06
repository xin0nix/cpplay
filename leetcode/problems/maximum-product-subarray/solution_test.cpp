#include <gtest/gtest.h>
#include <vector>

#include <ranges>
class Solution {
public:
  int maxProduct(std::vector<int> &nums) {
    // Будем хранить как наибольшее так и наименьшее (отрицательное)
    // произведение, тогда "переключаться" между кандидатами будет легко (-1 *
    // min может дать > max)
    int min = nums.front();
    int max = min;
    int res = min;
    for (auto num : nums | std::views::drop(1)) {
      if (num == 0) {
        // N[i+1] == N, то есть 0 не влияет на результат
        min = 1;
        max = 1;
        res = std::max(res, 0);
        continue;
      }
      int prodNumMin = num * min;
      int prodNumMax = num * max;
      // на каждом шаге i мы можем выбрать либо N[i] либо N[i-1] * N[i]
      min = std::min({prodNumMin, prodNumMax, num});
      max = std::max({prodNumMin, prodNumMax, num});
      res = std::max(res, max);
    }
    return res;
  }
};

// Define a test fixture for parametrized tests
class MaxProductSubarrayTest
    : public ::testing::TestWithParam<std::pair<std::vector<int>, int>> {
protected:
  Solution solution;
};

// Test cases using parametrized testing
TEST_P(MaxProductSubarrayTest, TestMaxProduct) {
  auto param = GetParam();
  std::vector<int> input = param.first;
  int expected = param.second;
  EXPECT_EQ(solution.maxProduct(input), expected);
}

// Instantiate test cases with various input scenarios
INSTANTIATE_TEST_SUITE_P(
    MaxProductSubarrayTests, MaxProductSubarrayTest,
    ::testing::Values(
        // Test case 1: Mix of positive and negative numbers
        std::make_pair(std::vector<int>{2, 3, -2, 4},
                       6), // Subarray [2, 3] gives 6 [7][8]
        // Test case 2: All negative numbers with zero
        std::make_pair(std::vector<int>{-2, 0, -1},
                       0), // Subarray [0] gives 0 [7][8]
        // Test case 3: All positive numbers
        std::make_pair(std::vector<int>{1, 2, 3, 4, 5, 0},
                       120), // Subarray [1,2,3,4,5] gives 120 [4]
        // Test case 4: Mix with multiple negatives yielding high product
        std::make_pair(std::vector<int>{2, 3, -2, -5, 6, -1, 4},
                       360), // Subarray [2,3,-2,-5,6] gives 360 [10]
        // Test case 5: Single element
        std::make_pair(std::vector<int>{-2}, -2), // Only element is -2 [2]
        // Test case 6: Two negative numbers
        std::make_pair(std::vector<int>{-2, -1},
                       2), // Subarray [-2, -1] gives 2 [3]
        // Test case 7: Array with zeros splitting potential subarrays
        std::make_pair(std::vector<int>{-1, -2, -3, 0, 3, 5},
                       15), // Subarray [3,5] gives 15 [8]
        // Test case 8: All negative numbers
        std::make_pair(std::vector<int>{-1, -2, -3},
                       6) // Subarray [-2, -3] gives 6 [8]
        ));
