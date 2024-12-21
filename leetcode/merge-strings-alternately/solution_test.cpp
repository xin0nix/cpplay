#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <sstream>
#include <string>

struct Solution {
  std::string mergeAlternately(const std::string &word1,
                               const std::string &word2) {
    std::stringstream ss;
    auto it1 = word1.cbegin();
    auto it2 = word2.cbegin();
    for (; it1 != word1.cend() && it2 != word2.cend(); ++it1, ++it2) {
      ss << *it1 << *it2;
    }
    auto ssIt = std::ostream_iterator<char>(ss);
    std::copy(it1, word1.cend(), ssIt);
    std::copy(it2, word2.cend(), ssIt);
    return ss.str();
  }
};

struct MergeStringsTest : testing::Test {};

// Test cases
TEST_F(MergeStringsTest, Example1) {
  EXPECT_EQ(Solution().mergeAlternately("abc", "pqr"), "apbqcr");
}

TEST_F(MergeStringsTest, Example2) {
  EXPECT_EQ(Solution().mergeAlternately("ab", "pqrs"), "apbqrs");
}

TEST_F(MergeStringsTest, Example3) {
  EXPECT_EQ(Solution().mergeAlternately("abcd", "pq"), "apbqcd");
}
