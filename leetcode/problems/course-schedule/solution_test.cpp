#include <gtest/gtest.h>
#include <tuple>
#include <unordered_set>
#include <vector>

using namespace std;

#include <ranges>
struct Solution {
  struct Impl {
    /*
      Функция buildAdjacencyList(P):
          Инициализировать пустой словарь A
          Для каждого i ∈ [0, numCurses)
              A[i] = []
          Для каждой пары [a, b] в P:
              Добавить b в A[a]
    */
    void buildAdjacencyMap(const int numCourses,
                           const vector<vector<int>> &prerequisites) {
      for (auto idx : std::views::iota(0, numCourses)) {
        dependenceMap[idx] = {};
      }
      for (const auto &targetPre : prerequisites) {
        int target = targetPre[0];
        int pre = targetPre[1];
        // > To take course a, you must first take course b.
        dependenceMap[target].emplace(pre);
      }
    }

    /*
      Функция DFS(N):
        Если N ∈ V:
            вернуть "нет"  // найден цикл
        Если N ∈ S:
            вернуть "да"   // узел уже обработан, циклов нет
        Если список смежности N пуст:
            вернуть "да"
        Добавить N в V
        Для каждого соседа bij из A[N]:
            Если DFS(bij) == "нет":
                вернуть "нет"
        Удалить N из V
        Добавить N в S
        вернуть "да"
    */
    bool dfs(int node, int level = 0) {
      // Indent output according to recursion level
      if (visiting.contains(node)) {
        return false;
      }

      auto &&adjList = dependenceMap[node];
      if (seen.contains(node) || adjList.empty()) {
        return true;
      }

      for (int adj : adjList) {
        visiting.emplace(node);
        if (!dfs(adj, level + 1)) {
          return false;
        }
        visiting.erase(node);
      }

      seen.emplace(node);
      return true;
    }

    /*
    Инициализировать множества S = ∅, V = ∅

    Для каждого узла N в графе:
        Если N ∉ S:
            Если DFS(N) == "нет":
                вернуть "нет"  // цикл найден, задача неразрешима

    Вернуть "да"  // циклов нет, все курсы можно пройти
     */
    bool canFinish(int numCourses, vector<vector<int>> &prerequisites) {
      buildAdjacencyMap(numCourses, prerequisites);
      for (auto idx : std::views::iota(0, numCourses)) {
        if (seen.contains(idx)) {
          continue;
        }
        visiting.clear();
        if (not dfs(idx)) {
          return false;
        }
      }
      return true;
    }

    std::unordered_map<int, std::set<int>> dependenceMap;
    std::unordered_set<int> seen;
    std::unordered_set<int> visiting;
  };

  bool canFinish(int numCourses, vector<vector<int>> &prerequisites) {
    return Impl{}.canFinish(numCourses, prerequisites);
  }
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
