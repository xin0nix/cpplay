#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <iostream>
#include <string_view>

using namespace std;

struct BruteForce {
  [[nodiscard]] int characterReplacement(string_view s, const int k) const noexcept {
    array<int, 26> freq = {};
    int res = 0;
    for (int l = 0, r = 0; r < s.size(); ++r) {
      int rFreqIdx = static_cast<int>(s[r] - 'A');
      int lFreqIdx = static_cast<int>(s[l] - 'A');
      freq[rFreqIdx]++;
      int mostFreq = 0;
      for (int f : freq)
        mostFreq = max(f, mostFreq);
      int len = r - l + 1;
      if (len - mostFreq <= k)
        res = max(res, len);
      else
        freq[lFreqIdx]--, ++l;
    }
    return res;
  }
};

TEST(LongestRepeatingCharacterReplacementTest, Empty) {
  ASSERT_EQ(BruteForce().characterReplacement("", 2), 0);
}

TEST(LongestRepeatingCharacterReplacementTest, NeetCodeExample) {
  ASSERT_EQ(BruteForce().characterReplacement("AABABBA", 2), 5);
}

TEST(LongestRepeatingCharacterReplacementTest, Monotonic1) {
  ASSERT_EQ(BruteForce().characterReplacement("ABCDEFGH", 0), 1);
}

TEST(LongestRepeatingCharacterReplacementTest, Monotonic2) {
  ASSERT_EQ(BruteForce().characterReplacement("ABCDEFGH", 1), 2);
}

TEST(LongestRepeatingCharacterReplacementTest, Monotonic3) {
  ASSERT_EQ(BruteForce().characterReplacement("ABCDEFGH", 2), 3);
}

TEST(LongestRepeatingCharacterReplacementTest, BugFix1) {
  ASSERT_EQ(BruteForce().characterReplacement("BAAA", 0), 3);
}
