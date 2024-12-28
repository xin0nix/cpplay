#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <ranges>
#include <stack>
#include <vector>

using namespace std;

class BruteForce {
public:
  int carFleet(int target, vector<int> &positions, vector<int> &velocities) {
    std::vector<pair<int, int>> cars;
    cars.reserve(positions.size());
    for (auto pos = positions.begin(), speed = velocities.begin();
         pos != positions.end(); pos++, speed++)
      cars.emplace_back(*pos, *speed);
    ranges::sort(cars.begin(), cars.end(), ranges::greater_equal{},
                 &std::pair<int, int>::first);
    // we can emulate stack, as we only track the top element and size
    int numFleets = 0;
    float fleetsTop = 0.0;
    for (auto [pos, speed] : cars) {
      double arrival = double(target - pos) / double(speed);
      if (numFleets == 0 || arrival > fleetsTop) {
        fleetsTop = arrival;
        ++numFleets;
      }
    }
    return numFleets;
  }
};

TEST(CarFleetTest, LeetCodeExample1) {
  vector<int> positions{10, 8, 0, 5, 3};
  vector<int> velocities{2, 4, 1, 1, 3};
  ASSERT_EQ(BruteForce().carFleet(12, positions, velocities), 3);
}

TEST(CarFleetTest, LeetCodeExample2) {
  vector<int> positions{3};
  vector<int> velocities{3};
  ASSERT_EQ(BruteForce().carFleet(10, positions, velocities), 1);
}

TEST(CarFleetTest, LeetCodeExample3) {
  vector<int> positions{0, 2, 4};
  vector<int> velocities{4, 2, 1};
  ASSERT_EQ(BruteForce().carFleet(100, positions, velocities), 1);
}

TEST(CarFleetTest, LeetCodeBug1) {
  vector<int> positions{0, 4, 2};
  vector<int> velocities{2, 1, 3};
  ASSERT_EQ(BruteForce().carFleet(10, positions, velocities), 1);
}
