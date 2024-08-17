#include <gmock/gmock.h>
#include <gtest/gtest.h>

using std::vector;
using testing::ElementsAre;

struct Solution {
  vector<int> replaceElements(vector<int> &arr) {
    if (arr.empty())
      return {};
    if (arr.size() == 1)
      return {-1};
    int max = arr.back();
    for (int i = arr.size() - 2; i >= 0; --i) {
      int prevMax = max;
      if (arr[i] > max)
        max = arr[i];
      arr[i] = prevMax;
    }
    arr.back() = -1;
    return std::move(arr);
  }
};

TEST(ReplaceElemenetsTest, Empty) {
  vector<int> arr{};
  ASSERT_THAT(Solution().replaceElements(arr), ElementsAre());
}

TEST(ReplaceElemenetsTest, Single) {
  vector arr{1};
  ASSERT_THAT(Solution().replaceElements(arr), ElementsAre(-1));
}

TEST(ReplaceElemenetsTest, Leet1) {
  vector arr{17, 18, 5, 4, 6, 1};
  ASSERT_THAT(Solution().replaceElements(arr), ElementsAre(18, 6, 6, 6, 1, -1));
}
