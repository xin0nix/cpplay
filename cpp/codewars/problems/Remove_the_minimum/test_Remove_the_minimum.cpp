#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

// Declaration of the function to be tested
std::vector<uint> removeSmallest(const std::vector<uint> &numbers) {
  if (numbers.empty()) {
    return {};
  }
  std::vector res(numbers);
  auto curMinIter = std::min_element(res.cbegin(), res.cend());
  res.erase(curMinIter);
  return res;
}

struct RemoveSmallestTestParam {
  std::vector<uint> input;
  std::vector<uint> expected;
};

class RemoveSmallestTest
    : public ::testing::TestWithParam<RemoveSmallestTestParam> {};

TEST_P(RemoveSmallestTest, RemovesLowestValueFirstOccurrence) {
  const auto &param = GetParam();
  auto result = removeSmallest(param.input);
  EXPECT_THAT(result, ::testing::ElementsAreArray(param.expected));
}

INSTANTIATE_TEST_SUITE_P(
    RemoveSmallestTests, RemoveSmallestTest,
    ::testing::Values(
        RemoveSmallestTestParam{{1, 2, 3, 4, 5}, {2, 3, 4, 5}},
        RemoveSmallestTestParam{{5, 3, 2, 1, 4}, {5, 3, 2, 4}},
        RemoveSmallestTestParam{{2, 2, 1, 2, 1}, {2, 2, 2, 1}},
        RemoveSmallestTestParam{{}, {}},            // empty input
        RemoveSmallestTestParam{{7}, {}},           // single element removed
        RemoveSmallestTestParam{{3, 3, 3}, {3, 3}}, // all same elements
        RemoveSmallestTestParam{{5, 4, 4, 5, 4}, {5, 4, 5, 4}}
        // multiple equal smallest
        ));
