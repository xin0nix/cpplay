#include <gtest/gtest.h>

#include <algorithm>
#include <fmt/core.h>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;

template <typename T>
concept PermutationSolution = requires(T a) {
  { a.checkInclusion(declval<string>(), declval<string>()) } -> same_as<bool>;
};

struct SolutionN26 {
  bool checkInclusion(string_view target, string_view message) {
    if (target.size() > message.size())
      return false;
    unordered_map<char, int> key;
    unordered_map<char, int> window;
    for (auto c : target)
      key[c]++;
    for (auto c : message.substr(0, target.size()))
      window[c]++;
    for (int l = 0, r = target.size(); r <= message.size(); ++l, ++r) {
      if (l > 0) {
        // [l, r)
        char cOut = message[l - 1];
        char cIn = message[r - 1];
        window[cOut] -= 1;
        window[cIn] += 1;
      }
      bool mismatch =
          (ranges::find_if_not(key.begin(), key.end(), [&](auto &p) -> bool {
             // do elements match ?
             auto &[k, v] = p;
             if (window[k] == v)
               return true;
             return false;
           }) != key.end());
      if (mismatch)
        continue;
      return true;
    }
    return false;
  }
};

struct SolutionN {
  struct PseudoHashMap {
    array<int, 26> data = {0};
    int &get(char c) { return data[(int)c - (int)'a']; }
  };

  bool checkInclusion(string_view target, string_view message) {
    if (target.size() > message.size())
      return false;
    PseudoHashMap tgtFreq;
    PseudoHashMap winFreq;
    for (auto c : target)
      tgtFreq.get(c)++;
    for (auto c : message.substr(0, target.size()))
      winFreq.get(c)++;
    int matches = 0;
    for (char c = 'a'; c <= 'z'; ++c)
      if (tgtFreq.get(c) == winFreq.get(c)) [[unlikely]]
        matches++;
    for (int l = 1, r = target.size() + 1; r <= message.size(); ++l, ++r) {
      if (matches == 26) [[unlikely]]
        return true;
      const char cOut = message[l - 1];
      const char cIn = message[r - 1];
      winFreq.get(cIn)++;
      if (winFreq.get(cIn) == tgtFreq.get(cIn))
        matches++;
      else if (winFreq.get(cIn) - 1 == tgtFreq.get(cIn))
        matches--;
      winFreq.get(cOut)--;
      if (winFreq.get(cOut) == tgtFreq.get(cOut))
        matches++;
      else if (winFreq.get(cOut) + 1 == tgtFreq.get(cOut))
        matches--;
    }
    return (matches == 26);
  }
};

template <PermutationSolution T>
struct PermutationInString : public testing::Test {
  using value_type = T;
  T value;
  T &getSolution() { return value; }
};

using Implementations = testing::Types<SolutionN, SolutionN26>;

TYPED_TEST_SUITE(PermutationInString, Implementations);

TYPED_TEST(PermutationInString, LeetCodeExample1) {
  ASSERT_TRUE(this->getSolution().checkInclusion("ab", "eidbaooo"));
}

TYPED_TEST(PermutationInString, LeetCodeExample2) {
  ASSERT_FALSE(this->getSolution().checkInclusion("ab", "eidboaoo"));
}

TYPED_TEST(PermutationInString, LeetCodeBug1) {
  ASSERT_TRUE(this->getSolution().checkInclusion("adc", "dcda"));
}

TYPED_TEST(PermutationInString, LeetCodeBug2) {
  ASSERT_TRUE(this->getSolution().checkInclusion("abc", "bbbca"));
}

TYPED_TEST(PermutationInString, LeetCodeBug3) {
  ASSERT_TRUE(this->getSolution().checkInclusion("a", "ab"));
}