#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class TimeMap {
  struct Item {
    std::string value;
    int timestamp;
  };
  std::unordered_map<std::string, std::vector<Item>> map_;

public:
  TimeMap() {}

  void set(std::string key, std::string value, int timestamp) {
    map_[std::move(key)].emplace_back(std::move(value), timestamp);
  }

  std::string get(const std::string &key, int timestamp) {
    auto it = map_.find(key);
    if (it == map_.end()) {
      return "";
    }
    auto &&vals = it->second;
    if (vals.empty() or timestamp < vals.front().timestamp) {
      return "";
    }
    int left = 0;
    int right = int(vals.size()) - 1;
    while (left < right) {
      int mid = (left + right + 1) / 2;
      if (vals[mid].timestamp <= timestamp) {
        left = mid;
      } else {
        right = mid - 1;
      }
    }
    return vals[left].value;
  }
};

using namespace std;

// Struct to hold test parameters
struct TimeMapTestParam {
  vector<tuple<string, string, int>> sets; // key, value, timestamp
  vector<tuple<string, int, string>> gets; // key, timestamp, expected_value
};

// Parameterized test fixture
class TimeMapTest : public ::testing::TestWithParam<TimeMapTestParam> {};

TEST_P(TimeMapTest, BasicOperations) {
  const auto &param = GetParam();
  TimeMap tm;

  // Perform set operations
  for (const auto &s : param.sets) {
    tm.set(get<0>(s), get<1>(s), get<2>(s));
  }

  // Perform get operations and check results
  for (const auto &g : param.gets) {
    string result = tm.get(get<0>(g), get<1>(g));
    EXPECT_EQ(result, get<2>(g))
        << "Failed on get(" << get<0>(g) << ", " << get<1>(g) << ")";
  }
}

// Instantiate test cases
INSTANTIATE_TEST_SUITE_P(
    TimeMapTests, TimeMapTest,
    ::testing::Values(
        // Basic test: single set/get
        TimeMapTestParam{{{"foo", "bar", 1}},
                         {{"foo", 1, "bar"}, {"foo", 2, "bar"}}},
        // Multiple sets for same key, different timestamps
        TimeMapTestParam{{{"foo", "bar", 1}, {"foo", "baz", 3}},
                         {{"foo", 1, "bar"},
                          {"foo", 2, "bar"},
                          {"foo", 3, "baz"},
                          {"foo", 4, "baz"}}},
        // No value before set
        TimeMapTestParam{{}, {{"foo", 1, ""}}},
        // Multiple keys
        TimeMapTestParam{{{"foo", "bar", 1}, {"baz", "qux", 2}},
                         {{"foo", 1, "bar"},
                          {"baz", 2, "qux"},
                          {"foo", 2, "bar"},
                          {"baz", 1, ""}}},
        // Overlapping timestamps
        TimeMapTestParam{{{"a", "x", 5}, {"a", "y", 10}, {"a", "z", 15}},
                         {{"a", 5, "x"},
                          {"a", 10, "y"},
                          {"a", 12, "y"},
                          {"a", 15, "z"},
                          {"a", 20, "z"},
                          {"a", 4, ""}}}));
