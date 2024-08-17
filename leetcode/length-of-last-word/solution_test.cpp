#include <algorithm>
#include <gtest/gtest.h>
#include <string>
using std::string;

struct Solution {
  int lengthOfLastWord(string s) {
    size_t lastWordLen = 0;
    auto it = s.cbegin();
    auto isSpace = [](char c) { return std::isspace(c); };
    while (it != s.cend()) {
      // Skip spaces
      it = std::find_if_not(it, s.cend(), isSpace);
      // Find where the word ends
      auto end = std::find_if(it, s.cend(), isSpace);
      // Measure the length (it might be zero)
      size_t len = static_cast<size_t>(end - it);
      if (len)
        lastWordLen = len;
      it = end;
    }
    return lastWordLen;
  }
};

TEST(LastWordLen, Leet) {
  EXPECT_EQ(Solution().lengthOfLastWord("Hello World"), 5);
  EXPECT_EQ(Solution().lengthOfLastWord("   fly me   to   the moon  "), 4);
  EXPECT_EQ(Solution().lengthOfLastWord("luffy is still joyboy"), 6);
}

TEST(LastWordLen, CornerCases) {
  EXPECT_EQ(Solution().lengthOfLastWord(""), 0);
  EXPECT_EQ(Solution().lengthOfLastWord("     "), 0);
  EXPECT_EQ(Solution().lengthOfLastWord("luffy"), 5);
  EXPECT_EQ(Solution().lengthOfLastWord("Today is a nice day"), 3);
}
