#include <gtest/gtest.h>
#include <set>
#include <vector>

class Solution {
public:
  std::vector<int>
  findOrder(int numCourses,
            const std::vector<std::vector<int>> &prerequisites) {
    // Build adj list map
    std::map<int, std::set<int>> adjList;
    for (int i = 0; i < numCourses; ++i) {
      adjList[i] = {};
    }
    for (auto &&prereq : prerequisites) {
      auto courseA = prereq.at(0);
      auto courseB = prereq.at(1);
      adjList.at(courseA).insert(courseB);
    }
    // Perform topological sort
    std::vector<int> ordering;
    // Set "processed" contains cources that we have fully processed
    std::set<int> processed;
    for (auto &&entry : adjList) {
      auto course = entry.first;
      // We've already fully processed this course - skip
      if (processed.contains(course)) {
        continue;
      }
      // Set "seen" contains courses we have seen along the current path
      std::set<int> onPath;
      std::function<bool(int)> dfs = [&](int cur) {
        // We've found a loop
        if (onPath.contains(cur)) {
          return false;
        }
        // We can stop at cources that either have no prerequisites
        // or have already been fully processed
        auto &&adjCources = adjList[cur];
        onPath.emplace(cur);
        for (auto adj : adjCources) {
          if (processed.contains(adj)) {
            continue;
          }
          // Otherwise - continue along the current path
          if (not dfs(adj)) {
            return false;
          }
        }
        onPath.erase(cur);
        ordering.push_back(cur);
        processed.emplace(cur);
        return true;
      };
      if (not dfs(course)) {
        return {};
      }
    }
    return ordering;
  }
};

struct FindOrderTestCase {
  int numCourses;
  std::vector<std::vector<int>> prerequisites;
  std::vector<int> expectedOrder;
};

class FindOrderTest : public ::testing::TestWithParam<FindOrderTestCase> {};

TEST_P(FindOrderTest, ReturnsCorrectOrder) {
  const auto &param = GetParam();
  Solution sol;
  // Uncomment and implement the following line in your solution file:
  auto result = sol.findOrder(param.numCourses, param.prerequisites);
  EXPECT_EQ(result, param.expectedOrder);
}

// Test cases
INSTANTIATE_TEST_SUITE_P(
    FindOrderTests, FindOrderTest,
    ::testing::Values(
        // No prerequisites, any order is valid
        FindOrderTestCase{2, {}, {0, 1}}, FindOrderTestCase{3, {}, {0, 1, 2}},
        // Simple linear dependency
        FindOrderTestCase{2, {{1, 0}}, {0, 1}},
        // Multiple dependencies
        FindOrderTestCase{4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, {0, 1, 2, 3}},
        // Cycle (impossible)
        FindOrderTestCase{2, {{0, 1}, {1, 0}}, {}},
        // Complex cycle
        FindOrderTestCase{3, {{0, 1}, {1, 2}, {2, 0}}, {}},
        // Disconnected graph
        FindOrderTestCase{4, {{1, 0}}, {0, 1, 2, 3}},
        // Single course
        FindOrderTestCase{1, {}, {0}},
        // No courses
        FindOrderTestCase{0, {}, {}},
        // LeetCode bug 1
        FindOrderTestCase{6,
                          {{2, 3}, {1, 2}, {0, 1}, {0, 4}, {4, 5}, {5, 1}},
                          {3, 2, 1, 5, 4, 0}}));
