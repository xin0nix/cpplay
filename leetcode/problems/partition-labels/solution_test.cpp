#include <gtest/gtest.h>
#include <string>
#include <vector>
using namespace std;

struct Solution {
  vector<int> partitionLabels(const string &word) {
    std::array<int, 26> hashMap = {0};
    auto atHashMap = [&](char letter) -> int & {
      int index = letter - 'a';
      return hashMap.at(index);
    };
    for (int pos = 0, len = int(word.size()); pos < len; ++pos) {
      atHashMap(word.at(pos)) = pos;
    }
    int size{0};
    int end{0};
    std::vector<int> partitions;
    for (int pos = 0, len = int(word.size()); pos < len; ++pos) {
      size += 1;
      auto letter = word.at(pos);
      auto lastPos = atHashMap(letter);
      end = std::max(end, lastPos);
      if (pos == end) {
        partitions.push_back(size);
        size = 0;
      }
    }
    return partitions;
  }
};

struct PartitionLabelsTestCase {
  string input;
  vector<int> expected;
};

class PartitionLabelsTest
    : public ::testing::TestWithParam<PartitionLabelsTestCase> {};

TEST_P(PartitionLabelsTest, HandlesVariousInputs) {
  Solution sol;
  const auto &param = GetParam();
  vector<int> result = sol.partitionLabels(param.input);
  EXPECT_EQ(result, param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    PartitionLabelsTests, PartitionLabelsTest,
    ::testing::Values(
        // Original LeetCode example
        PartitionLabelsTestCase{"ababcbacadefegdehijhklij", {9, 7, 8}},
        // Single partition with repeated chars
        PartitionLabelsTestCase{"eccbbbbdec", {10}},
        // Two partitions
        PartitionLabelsTestCase{"caedbdedda", {1, 9}},
        // All distinct letters, each letter its own partition
        PartitionLabelsTestCase{"abc", {1, 1, 1}},
        // Empty string edge case
        PartitionLabelsTestCase{"", {}},
        // Whole alphabet once, each letter should be its own partition
        PartitionLabelsTestCase{"abcdefghijklmnopqrstuvwxyz",
                                vector<int>(26, 1)},
        // Alphabet repeated twice consecutively, partitions merge per letter
        PartitionLabelsTestCase{
            "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", {52}},
        // Repeated pattern with overlapping letters
        PartitionLabelsTestCase{"abacbdade", {8, 1}},
        // String with one letter repeated multiple times
        PartitionLabelsTestCase{"aaaaaa", {6}},
        // String with letters appearing in separated blocks forcing larger
        // partitions
        PartitionLabelsTestCase{"abacdcbed", {9}},
        // String with multiple small partitions
        PartitionLabelsTestCase{"aabbccddeeff", {2, 2, 2, 2, 2, 2}}));
