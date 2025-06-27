#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

using namespace std;

class Interval {
public:
  int start, end;
  Interval(int start, int end) : start(start), end(end) {}
};

class Solution {
public:
  bool canAttendMeetings(vector<Interval> &intervals) {
    std::sort(intervals.begin(), intervals.end(),
              [](auto &lhs, auto &rhs) { return lhs.start < rhs.start; });
    for (int first = 0; first + 1 < intervals.size(); ++first) {
      int second = first + 1;
      if (intervals.at(first).end > intervals.at(second).start) {
        return false;
      }
    }
    return true;
  }
};

class MeetingRoomsTest
    : public testing::TestWithParam<tuple<vector<tuple<int, int>>, bool>> {
protected:
  void SetUp() override {
    auto params = GetParam();
    auto intervalData = get<0>(params);
    expected = get<1>(params);

    for (auto &data : intervalData) {
      intervals.emplace_back(get<0>(data), get<1>(data));
    }
  }

  vector<Interval> intervals;
  bool expected;
};

TEST_P(MeetingRoomsTest, ValidatesMeetingAttendability) {
  Solution sol;
  ASSERT_EQ(sol.canAttendMeetings(intervals), expected);
}

INSTANTIATE_TEST_SUITE_P(
    MeetingRoomsTestCases, MeetingRoomsTest,
    testing::Values(
        // Empty intervals
        make_tuple(vector<tuple<int, int>>{}, true),

        // Single interval
        make_tuple(vector<tuple<int, int>>{{1, 5}}, true),

        // Non-overlapping intervals
        make_tuple(vector<tuple<int, int>>{{0, 5}, {6, 10}, {11, 15}}, true),

        // Overlapping intervals
        make_tuple(vector<tuple<int, int>>{{0, 5}, {3, 10}}, false),
        make_tuple(vector<tuple<int, int>>{{1, 3}, {2, 4}}, false),

        // Adjacent but non-overlapping
        make_tuple(vector<tuple<int, int>>{{1, 2}, {2, 3}}, true),

        // Intervals with same start time
        make_tuple(vector<tuple<int, int>>{{1, 3}, {1, 4}}, false),

        // Contained interval
        make_tuple(vector<tuple<int, int>>{{0, 10}, {3, 5}}, false),

        // Multiple overlaps
        make_tuple(vector<tuple<int, int>>{{1, 5}, {2, 3}, {4, 7}}, false),

        // Large numbers
        make_tuple(vector<tuple<int, int>>{{100, 1000}, {1001, 2000}}, true),
        make_tuple(vector<tuple<int, int>>{{100, 1000}, {999, 2000}}, false)));
