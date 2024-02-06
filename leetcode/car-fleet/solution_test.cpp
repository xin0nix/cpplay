#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

using namespace std;

class Solution {
public:
  int carFleet(int target, vector<int> &position, vector<int> &speed) {
    __builtin_unreachable();
  }
};

TEST(CarFleetTest, LeetCodeExample1) {
  vector<int> positions{10, 8, 0, 5, 3};
  vector<int> velocities{2, 4, 1, 1, 3};
  ASSERT_EQ(Solution().carFleet(12, positions, velocities), 3);
}

TEST(CarFleetTest, LeetCodeExample2) {
  vector<int> positions{3};
  vector<int> velocities{3};
  ASSERT_EQ(Solution().carFleet(10, positions, velocities), 1);
}

TEST(CarFleetTest, LeetCodeExample3) {
  vector<int> positions{0, 2, 4};
  vector<int> velocities{4, 2, 1};
  ASSERT_EQ(Solution().carFleet(100, positions, velocities), 1);
}
