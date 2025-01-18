#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

using ::testing::ElementsAre;

struct Solution {
  void reverseString(std::vector<char> &str) {
    if (str.size() <= 1) [[unlikely]] {
      return;
    }
    for (auto left = 0UL, right = str.size() - 1; left < right;
         left++, right--) {
      std::swap(str[left], str[right]);
    }
  }
};

struct ReverseStringTest : ::testing::Test {};

TEST_F(ReverseStringTest, Empty) {
  std::vector<char> str = {};
  Solution{}.reverseString(str);
  EXPECT_THAT(str, ElementsAre());
}

TEST_F(ReverseStringTest, Single) {
  std::vector str = {'h'};
  Solution{}.reverseString(str);
  EXPECT_THAT(str, ElementsAre('h'));
}

TEST_F(ReverseStringTest, Double) {
  std::vector str = {'h', 'e'};
  Solution{}.reverseString(str);
  EXPECT_THAT(str, ElementsAre('e', 'h'));
}

TEST_F(ReverseStringTest, Three) {
  std::vector str = {'h', 'e', 'l'};
  Solution{}.reverseString(str);
  EXPECT_THAT(str, ElementsAre('l', 'e', 'h'));
}

TEST_F(ReverseStringTest, Leet1) {
  std::vector str = {'h', 'e', 'l', 'l', 'o'};
  Solution{}.reverseString(str);
  EXPECT_THAT(str, ElementsAre('o', 'l', 'l', 'e', 'h'));
}