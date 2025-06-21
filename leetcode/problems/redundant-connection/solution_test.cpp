#include <gtest/gtest.h>

#include <cstdint>
#include <stdexcept>
#include <vector>
using namespace std;

class Solution {
  struct Node {
    Node(std::uint32_t n) : parent(n) {}
    std::uint32_t parent;
    std::uint32_t size{0};
  };

  std::vector<Node> nodes;

  std::uint32_t findRoot(std::uint32_t idx) {
    auto root = idx;
    // node is a root if it references itself
    while (nodes.at(root).parent != root) {
      auto &node = nodes.at(root);
      node.parent = nodes.at(node.parent).parent;
      root = node.parent;
    }
    return root;
  }

  bool mergeAndDetectLoop(std::uint32_t left, std::uint32_t right) {
    auto leftRoot = findRoot(left);
    auto rightRoot = findRoot(right);
    if (leftRoot == rightRoot) {
      // We found a loop
      return true;
    }
    // The left tree should be smalle
    if (nodes.at(leftRoot).size > nodes.at(rightRoot).size) {
      std::swap(leftRoot, rightRoot);
    }
    nodes.at(leftRoot).parent = rightRoot;
    nodes.at(rightRoot).size += nodes.at(leftRoot).size;
    return false;
  }

public:
  vector<int> findRedundantConnection(const vector<vector<int>> &edges) {
    nodes.reserve(edges.size());
    for (int i = 0; i < edges.size(); ++i) {
      nodes.emplace_back(i);
    }
    for (const auto &edge : edges) {
      if (mergeAndDetectLoop(edge.at(0) - 1, edge.at(1) - 1)) {
        return edge;
      }
    }
    throw std::logic_error("No cycles detected");
  }
};

// Define a struct to hold each test case's parameters.
struct RedundantConnectionTestCase {
  vector<vector<int>> edges;
  vector<int> expected;
};

// Test fixture for parameterized tests.
class RedundantConnectionTest
    : public ::testing::TestWithParam<RedundantConnectionTestCase> {};

TEST_P(RedundantConnectionTest, HandlesVariousGraphs) {
  const auto &param = GetParam();
  Solution sol;
  vector<int> result = sol.findRedundantConnection(param.edges);
  ASSERT_EQ(result, param.expected);
}

// List of test cases for parameterization.
INSTANTIATE_TEST_SUITE_P(
    RedundantConnectionTests, RedundantConnectionTest,
    ::testing::Values(
        // Basic 3-node cycle
        RedundantConnectionTestCase{{{1, 2}, {1, 3}, {2, 3}}, {2, 3}},
        // 4-node cycle, redundant edge at the end
        RedundantConnectionTestCase{{{1, 2}, {2, 3}, {3, 4}, {1, 4}, {1, 5}},
                                    {1, 4}},
        // 5-node cycle, redundant edge in the middle
        RedundantConnectionTestCase{{{1, 2}, {2, 3}, {3, 4}, {4, 1}, {1, 5}},
                                    {4, 1}},
        // Redundant edge at the end, larger graph
        RedundantConnectionTestCase{{{1, 2}, {2, 3}, {3, 4}, {4, 5}, {2, 5}},
                                    {2, 5}},
        // Star graph with one extra edge forming a cycle
        RedundantConnectionTestCase{{{1, 2}, {1, 3}, {1, 4}, {4, 2}}, {4, 2}},
        // Minimal cycle (triangle)
        RedundantConnectionTestCase{{{1, 2}, {2, 3}, {3, 1}}, {3, 1}},
        // Redundant edge not the last one in input
        RedundantConnectionTestCase{{{1, 2}, {2, 3}, {3, 1}, {4, 5}}, {3, 1}}
        // Add more cases as needed for further coverage
        ));
