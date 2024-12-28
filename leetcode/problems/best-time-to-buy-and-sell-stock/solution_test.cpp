#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

using namespace std;
using namespace testing;

class BruteForce {
public:
  int maxProfit(vector<int> &prices) {
    int profit = 0;
    for (int l = 0, r = 0; r < prices.size(); ++r) {
      int a = prices[l], b = prices[r];
      if (a >= b) {
        l = r;
        continue;
      }
      profit = max(profit, b - a);
    }
    return profit;
  }
};

using namespace std;

TEST(MaxProfitTest, Empty) {
  vector<int> prices{};
  EXPECT_EQ(BruteForce().maxProfit(prices), 0);
}

TEST(MaxProfitTest, Basic1) {
  vector<int> prices{7, 1, 5, 3, 6, 4};
  EXPECT_EQ(BruteForce().maxProfit(prices), 5);
}

TEST(MaxProfitTest, Basic2) {
  vector<int> prices{7, 6, 4, 3, 1};
  EXPECT_EQ(BruteForce().maxProfit(prices), 0);
}
