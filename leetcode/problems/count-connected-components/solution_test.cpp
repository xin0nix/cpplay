#include <cstdint>
#include <gtest/gtest.h>
#include <sys/types.h>
#include <vector>
using std::vector;

class Solution {
  struct Node {
    Node(int n) : parent(n) {}
    std::uint32_t parent;
    std::uint32_t size{0};
  };

  std::vector<Node> nodes;

  std::uint32_t findRoot(std::uint32_t idx) {
    auto root = idx;
    while (nodes.at(root).parent != root) {
      auto &node = nodes.at(root);
      node.parent = nodes.at(node.parent).parent;
      root = node.parent;
    }
    return root;
  }

  bool mergeIfDisjoint(std::uint32_t left, std::uint32_t right) {
    auto rootLeft = findRoot(left);
    auto rootRight = findRoot(right);
    if (rootLeft == rootRight) {
      return false; // already in the same set
    }
    // otherwise - need to merge left tree into the right one
    auto &leftTree = nodes.at(rootLeft);
    auto &rightTree = nodes.at(rootRight);
    if (leftTree.size > rightTree.size) {
      std::swap(rootLeft, rootRight);
    }
    leftTree.parent = rootRight;
    rightTree.size = leftTree.size + rightTree.size;
    return true;
  }

public:
  int countComponents(int count, const vector<vector<int>> &edges) {
    // Create trees
    nodes.clear();
    nodes.reserve(count);
    for (int i = 0; i < count; ++i) {
      nodes.emplace_back(i);
    }
    // Merge and count
    for (const auto &edge : edges) {
      auto left = edge.at(0);
      auto right = edge.at(1);
      if (mergeIfDisjoint(left, right)) {
        count--; // merged means we have one component less
      }
    }
    return count;
  }
};

struct TestCase {
  int n;
  vector<vector<int>> edges;
  int expected;
};

class CountComponentsTest : public ::testing::TestWithParam<TestCase> {};

TEST_P(CountComponentsTest, HandlesVariousGraphs) {
  const TestCase &testCase = GetParam();
  Solution sol;
  EXPECT_EQ(sol.countComponents(testCase.n, testCase.edges), testCase.expected);
}

INSTANTIATE_TEST_SUITE_P(
    CountComponentsTests, CountComponentsTest,
    ::testing::Values(
        // No edges, all nodes disconnected
        TestCase{5, {}, 5},
        // Single connected component (chain)
        TestCase{5, {{0, 1}, {1, 2}, {2, 3}, {3, 4}}, 1},
        // Two connected components
        TestCase{5, {{0, 1}, {1, 2}, {3, 4}}, 2},
        // All nodes connected (complete graph)
        TestCase{4, {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}}, 1},
        // Disconnected pairs
        TestCase{4, {{0, 1}, {2, 3}}, 2},
        // Single node
        TestCase{1, {}, 1},
        // Empty graph
        TestCase{0, {}, 0},
        // Star topology
        TestCase{5, {{0, 1}, {0, 2}, {0, 3}, {0, 4}}, 1},
        // Cycle
        TestCase{4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}, 1},
        // Multiple components, some isolated nodes
        TestCase{6, {{0, 1}, {2, 3}}, 4}));
