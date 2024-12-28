#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <deque>
#include <stack>
#include <vector>

using namespace std;
using namespace testing;

class BruteForce {
public:
  vector<int> dailyTemperatures(vector<int> &temperatures) {
    // stack of monotonically decreasing temperature records
    stack<pair<size_t, int>> seen; // ids, temperature
    vector<int> res(temperatures.size(), 0);
    for (size_t i = 0, e = temperatures.size(); i < e; ++i) {
      int val = temperatures[i];
      while (seen.size()) {
        if (auto &[seenIdx, seenTemp] = seen.top(); seenTemp < val) {
          res[seenIdx] = i - seenIdx;
          seen.pop();
          continue;
        }
        break;
      }
      seen.push({i, val});
    }
    return res;
  }
};

using namespace std;

TEST(DailyTemperaturesTest, Empty) {
  vector<int> temperatures{};
  EXPECT_THAT(BruteForce().dailyTemperatures(temperatures), ElementsAre());
}

TEST(DailyTemperaturesTest, Basic1) {
  vector<int> temperatures{73, 74, 75, 71, 69, 72, 76, 73};
  EXPECT_THAT(BruteForce().dailyTemperatures(temperatures),
              ElementsAre(1, 1, 4, 2, 1, 1, 0, 0));
}

TEST(DailyTemperaturesTest, Basic2) {
  vector<int> temperatures{30, 40, 50, 60};
  EXPECT_THAT(BruteForce().dailyTemperatures(temperatures),
              ElementsAre(1, 1, 1, 0));
}

TEST(DailyTemperaturesTest, Basic3) {
  vector<int> temperatures{90, 90, 90};
  EXPECT_THAT(BruteForce().dailyTemperatures(temperatures), ElementsAre(0, 0, 0));
}
