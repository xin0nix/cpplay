#include <gtest/gtest.h>

#include <numeric>
#include <vector>

class BruteForce {
public:
  int pivotIndex(std::vector<int> &nums) {
    auto rightSum = std::accumulate(nums.cbegin(), nums.cend(), 0);
    auto leftSum = 0;
    for (auto idx = 0UL; idx < nums.size(); ++idx) {
      auto pivotal = nums[idx];
      // leftSum представляет собой сумму чисел для индексов
      // (-1,) 0, 1, ..., i-1 (nums[-1] примимается равной 0)
      // rightSum это сумма чисел справа, для индексов
      // i, i+1, ..., n
      // По определению pivotal index мы долждны исключить nums[i]
      // То есть получить
      // i+1, ..., n
      // В итоге получаем следующую проверку:
      if (leftSum == rightSum - pivotal) {
        return (int)idx;
      }
      // на следующей итерации это будет i-1
      leftSum += pivotal;
      rightSum -= pivotal;
    }
    return -1;
  }
};

struct PivotIndexTest : ::testing::Test {};

TEST_F(PivotIndexTest, Basic) {
  std::vector nums{
      1,
  };
  auto idx = BruteForce().pivotIndex(nums);
  ASSERT_EQ(idx, 0);
}

TEST_F(PivotIndexTest, Leet1) {
  std::vector nums{1, 7, 3, 6, 5, 6};
  auto idx = BruteForce().pivotIndex(nums);
  ASSERT_EQ(idx, 3);
}

TEST_F(PivotIndexTest, Leet2) {
  std::vector nums{1, 2, 3};
  auto idx = BruteForce().pivotIndex(nums);
  ASSERT_EQ(idx, -1);
}

TEST_F(PivotIndexTest, Leet2Reverse) {
  std::vector nums{3, 2, 1};
  auto idx = BruteForce().pivotIndex(nums);
  ASSERT_EQ(idx, -1);
}

TEST_F(PivotIndexTest, Leet3) {
  std::vector nums{2, 1, -1};
  auto idx = BruteForce().pivotIndex(nums);
  ASSERT_EQ(idx, 0);
}

TEST_F(PivotIndexTest, Leet3Reverse) {
  std::vector nums{-1, 1, 2};
  auto idx = BruteForce().pivotIndex(nums);
  ASSERT_EQ(idx, 2);
}