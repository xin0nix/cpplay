#include <gtest/gtest.h>

#include <vector>

class Solution {
  struct Node {
    Node(size_t parent_) : parent(parent_) {}
    size_t parent;
    size_t size{1};
  };

  size_t findRoot(size_t nodeIdx) {
    while (nodes.at(nodeIdx).parent != nodeIdx) {
      auto &node = nodes.at(nodeIdx);
      node.parent = nodes.at(node.parent).parent;
      nodeIdx = node.parent;
    }
    return nodeIdx;
  }

  // false - no changes, true - changed ok
  bool mergeDisjoint(size_t nodeX, size_t nodeY) {
    nodeX = findRoot(nodeX);
    nodeY = findRoot(nodeY);

    if (nodeX == nodeY) {
      return false;
    }

    if (nodes.at(nodeY).size > nodes.at(nodeX).size) {
      std::swap(nodeY, nodeX);
    }

    nodes.at(nodeY).parent = nodeX;
    nodes.at(nodeX).size += nodes.at(nodeY).size;
    return true;
  }

  std::vector<Node> nodes;

public:
  bool validTree(int n, const std::vector<std::vector<int>> &edges) {
    nodes.reserve(n);
    for (size_t root = 0UL; root < n; ++root) {
      nodes.emplace_back(root);
    }
    for (auto &&edge : edges) {
      size_t source = edge.at(0);
      size_t dest = edge.at(1);
      if (not mergeDisjoint(source, dest)) {
        return false; // already merged
      }
      n -= 1;
    }
    return n == 1;
  }
};

using namespace std;

// Define the struct for test parameters
struct ValidTreeTestCase {
  int n;
  vector<vector<int>> edges;
  bool expected;
};

// The test fixture
class ValidTreeTest : public ::testing::TestWithParam<ValidTreeTestCase> {};

// The parameterized test
TEST_P(ValidTreeTest, HandlesVariousGraphs) {
  const auto &param = GetParam();
  Solution sol;
  EXPECT_EQ(sol.validTree(param.n, param.edges), param.expected);
}

// Test cases
INSTANTIATE_TEST_SUITE_P(
    ValidTreeTests, ValidTreeTest,
    ::testing::Values(
        // Test case 1: Simple tree
        ValidTreeTestCase{5, {{0, 1}, {0, 2}, {0, 3}, {1, 4}}, true},
        // Test case 2: Contains a cycle
        ValidTreeTestCase{5, {{0, 1}, {1, 2}, {2, 3}, {1, 3}, {1, 4}}, false},
        // Test case 3: Disconnected graph
        ValidTreeTestCase{4, {{0, 1}, {2, 3}}, false},
        // Test case 4: Single node
        ValidTreeTestCase{1, {}, true},
        // Test case 5: Two nodes, one edge
        ValidTreeTestCase{2, {{0, 1}}, true},
        // Test case 6: Two nodes, no edge
        ValidTreeTestCase{2, {}, false}));
