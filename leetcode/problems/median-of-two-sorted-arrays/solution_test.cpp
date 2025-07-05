#include <gtest/gtest.h>
#include <tuple>
#include <vector>
using namespace std;

class Solution {
public:
  double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2) {
    if (nums1.size() > nums2.size()) {
      std::swap(nums1, nums2); // Зачем менять массивы местами?
    }
    int size1 = nums1.size();
    int size2 = nums2.size();
    bool isOdd = (size1 + size2) % 2 == 1;
    auto leftPartition = (size1 + size2 + 1) / 2;
    auto low = 0;
    auto high = size1; // Почему не n1-1 ?
    bool done = false;
    auto atPos = [](auto &arr, auto idx) {
      if (idx < 0) {
        return std::numeric_limits<int>::min();
      }
      if (idx >= arr.size()) {
        return std::numeric_limits<int>::max();
      }
      return arr.at(idx);
    };
    while (low <= high) {
      auto mid1 = (low + high) / 2;
      auto mid2 = leftPartition - mid1;
      auto lVal1 = atPos(nums1, mid1 - 1);
      auto lVal2 = atPos(nums2, mid2 - 1);
      auto rVal1 = atPos(nums1, mid1);
      auto rVal2 = atPos(nums2, mid2);
      if (lVal1 > rVal2) {
        high = mid1 - 1;
        continue;
      }
      if (lVal2 > rVal1) {
        low = mid1 + 1;
        continue;
      }
      if (isOdd) {
        return std::max(lVal1, lVal2);
      }
      return double(std::max(lVal1, lVal2) + std::min(rVal1, rVal2)) / 2.;
    }
    return 0.0;
  }
};

class MedianOfTwoSortedArraysTest
    : public ::testing::TestWithParam<tuple<vector<int>, vector<int>, double>> {
};

TEST_P(MedianOfTwoSortedArraysTest, HandlesVariousCases) {
  vector<int> nums1 = get<0>(GetParam());
  vector<int> nums2 = get<1>(GetParam());
  double expected = get<2>(GetParam());
  Solution sol;
  double result = sol.findMedianSortedArrays(nums1, nums2);
  EXPECT_DOUBLE_EQ(result, expected);
}

INSTANTIATE_TEST_SUITE_P(
    MedianTests, MedianOfTwoSortedArraysTest,
    ::testing::Values(
        // Basic examples
        make_tuple(vector<int>{1, 3}, vector<int>{2}, 2.0),
        make_tuple(vector<int>{1, 2}, vector<int>{3, 4}, 2.5),

        // Edge cases with zeros and negatives
        make_tuple(vector<int>{0, 0}, vector<int>{0, 0}, 0.0),
        make_tuple(vector<int>{-5, 3, 6, 12, 15},
                   vector<int>{-12, -10, -6, -3, 4, 10}, 3.0),
        make_tuple(vector<int>{-10, -5, 0, 5}, vector<int>{-3, 1, 2, 3, 4}, 1),

        // One empty array
        make_tuple(vector<int>{}, vector<int>{1}, 1.0),
        make_tuple(vector<int>{2}, vector<int>{}, 2.0),

        // Single-element arrays
        make_tuple(vector<int>{1}, vector<int>{2}, 1.5),
        make_tuple(vector<int>{5}, vector<int>{}, 5.0),

        // Unequal lengths
        make_tuple(vector<int>{1, 2, 3, 4, 5},
                   vector<int>{6, 7, 8, 9, 10, 11, 12}, 6.5),
        make_tuple(vector<int>{1, 3, 8, 9, 15},
                   vector<int>{7, 11, 18, 19, 21, 25}, 11.0),

        // Duplicates
        make_tuple(vector<int>{1, 1, 1, 1}, vector<int>{1, 1, 1, 1}, 1.0),

        // One array fully smaller than the other
        make_tuple(vector<int>{1, 2, 3}, vector<int>{10, 11, 12}, 6.5),

        // Large difference in size
        make_tuple(vector<int>{1}, vector<int>{2, 3, 4, 5, 6, 7, 8, 9, 10},
                   5.5)));
