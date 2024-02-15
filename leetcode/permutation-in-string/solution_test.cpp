#include <gtest/gtest.h>

#include <algorithm>
#include <fmt/core.h>
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

template <PermutationSolution T>
struct PermutationInString : public testing::Test {
  using value_type = T;
  T value;
  T &getSolution() { return value; }
};

using Implementations = testing::Types<SolutionN26>;

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