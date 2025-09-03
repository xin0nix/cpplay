// Assuming Kata class is declared in kata.h
#include <gtest/gtest.h>
#include <map>
#include <vector>

struct Kata {
  std::vector<int> sortArray(std::vector<int> array) {
    std::map<int, int> sorted;
    for (auto num : array) {
      if (num % 2 == 1) {
        sorted[num]++;
      }
    }
    auto iter = sorted.begin();
    for (auto &num : array) {
      if (num % 2 == 0) {
        continue;
      }
      if (iter->second == 0) {
        ++iter;
      }
      num = iter->first;
      iter->second--;
    }

    return array;
  }
};

class KataTest : public ::testing::Test {
protected:
  Kata kata;
};

TEST_F(KataTest, BasicTest) {
  // Must return odd numbers sorted ascending, even numbers unchanged and in
  // original positions
  std::vector<int> input1{7, 1};
  std::vector<int> expected1{1, 7};
  EXPECT_EQ(kata.sortArray(input1), expected1);

  std::vector<int> input2{5, 8, 6, 3, 4};
  std::vector<int> expected2{3, 8, 6, 5, 4};
  EXPECT_EQ(kata.sortArray(input2), expected2);

  std::vector<int> input3{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  std::vector<int> expected3{1, 8, 3, 6, 5, 4, 7, 2, 9, 0};
  EXPECT_EQ(kata.sortArray(input3), expected3);
}

TEST_F(KataTest, EmptyArray) {
  std::vector<int> input{};
  std::vector<int> expected{};
  EXPECT_EQ(kata.sortArray(input), expected);
}

TEST_F(KataTest, AllEvenNumbers) {
  std::vector<int> input{2, 4, 6, 8};
  std::vector<int> expected{2, 4, 6, 8}; // unchanged
  EXPECT_EQ(kata.sortArray(input), expected);
}

TEST_F(KataTest, AllOddNumbers) {
  std::vector<int> input{9, 3, 7, 1};
  std::vector<int> expected{1, 3, 7, 9}; // fully sorted
  EXPECT_EQ(kata.sortArray(input), expected);
}

TEST_F(KataTest, SingleElementOdd) {
  std::vector<int> input{5};
  std::vector<int> expected{5};
  EXPECT_EQ(kata.sortArray(input), expected);
}

TEST_F(KataTest, SingleElementEven) {
  std::vector<int> input{4};
  std::vector<int> expected{4};
  EXPECT_EQ(kata.sortArray(input), expected);
}
