#include <gtest/gtest.h>
#include <tuple>
#include <vector>

using namespace std;

// Forward declaration of the Solution class
class Solution {
public:
  bool canFinish(int numCourses, vector<vector<int>> &prerequisites) {}
};

// Type alias for test parameters: (numCourses, prerequisites, expected)
using TestParam = std::tuple<int, vector<vector<int>>, bool>;

// Helper functions for test cases

TestParam SingleNodeNoPrereq() { return {1, {}, true}; }

TestParam SingleNodeSelfCycle() { return {1, {{0, 0}}, false}; }

TestParam TwoNodesNoCycle() { return {2, {{1, 0}}, true}; }

TestParam TwoNodesWithCycle() { return {2, {{1, 0}, {0, 1}}, false}; }

TestParam DisconnectedGraphsNoCycle() { return {4, {{1, 0}, {3, 2}}, true}; }

TestParam DisconnectedGraphsWithCycle() {
  return {4, {{1, 0}, {3, 2}, {2, 3}}, false};
}

TestParam SimpleTree() { return {4, {{1, 0}, {2, 0}, {3, 1}}, true}; }

TestParam SimpleCycle() { return {3, {{0, 1}, {1, 2}, {2, 0}}, false}; }

TestParam MultipleIsolatedGraphs() {
  return {6, {{1, 0}, {3, 2}, {5, 4}}, true};
}

TestParam MultipleIsolatedGraphsWithCycle() {
  return {6, {{1, 0}, {3, 2}, {5, 4}, {4, 5}}, false};
}

TestParam NoPrerequisites() { return {5, {}, true}; }

TestParam AllToOne() { return {4, {{1, 0}, {2, 0}, {3, 0}}, true}; }

TestParam AllCycle() { return {3, {{0, 1}, {1, 2}, {2, 0}}, false}; }

// Parameterized test fixture
class CourseScheduleTest : public ::testing::TestWithParam<TestParam> {};

TEST_P(CourseScheduleTest, CanFinish) {
  int numCourses;
  vector<vector<int>> prerequisites;
  bool expected;
  std::tie(numCourses, prerequisites, expected) = GetParam();

  Solution sol;
  EXPECT_EQ(sol.canFinish(numCourses, prerequisites), expected);
}

// Instantiate test suite with all cases
INSTANTIATE_TEST_SUITE_P(
    CourseScheduleCases, CourseScheduleTest,
    ::testing::Values(SingleNodeNoPrereq(), SingleNodeSelfCycle(),
                      TwoNodesNoCycle(), TwoNodesWithCycle(),
                      DisconnectedGraphsNoCycle(),
                      DisconnectedGraphsWithCycle(), SimpleTree(),
                      SimpleCycle(), MultipleIsolatedGraphs(),
                      MultipleIsolatedGraphsWithCycle(), NoPrerequisites(),
                      AllToOne(), AllCycle()));
