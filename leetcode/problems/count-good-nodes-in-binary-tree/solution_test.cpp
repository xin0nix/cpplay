#include <gtest/gtest.h>

#include <array>
#include <deque>
#include <limits>

// Definition for a binary tree node.
struct TreeNode {
  TreeNode(const TreeNode &) noexcept = default;
  TreeNode(TreeNode &&) noexcept = default;
  TreeNode &operator=(const TreeNode &) noexcept = default;
  TreeNode &operator=(TreeNode &&) noexcept = default;
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
};

constexpr bool isValidTreeSize(size_t s) {
  // Check if s is of the form 2^N - 1
  size_t lowerMask = s - 1;
  size_t upperMask = s + 1;
  return (s & lowerMask) == lowerMask and (s & upperMask) == 0;
}

template <size_t N>
  requires(isValidTreeSize(N)) // that's not enough actually
struct Tree {
  std::array<TreeNode, N> data;

  explicit Tree(const std::array<std::optional<int>, N> &vals) {
    std::transform(vals.cbegin(), vals.cend(), data.begin(), data.begin(),
                   [](std::optional<int> x, TreeNode &node) -> TreeNode {
                     node.val = x.value_or(0);
                     return node;
                   });
    // parent:     (i - 1) / 2
    // left child:  p * 2 + 1
    // right child: p * 2 + 2
    for (int p = 0; p < N / 2; ++p) {
      const int l = p * 2 + 1;
      const int r = p * 2 + 2;
      if (vals[l].has_value())
        data[p].left = &data[l];
      if (vals[r].has_value())
        data[p].right = &data[r];
    }
  }

  TreeNode *head() {
    static_assert(N > 0);
    return &data.front();
  }
};

struct Solution {
  int goodNodes(TreeNode *root) {
    int count = 0;
    struct Entry {
      TreeNode *node;
      int maxVal;
    };
    std::deque<Entry> nodeQueue{{root, std::numeric_limits<int>::min()}};
    while (not nodeQueue.empty()) {
      auto [node, maxVal] = nodeQueue.front();
      nodeQueue.pop_front();
      if (node->val >= maxVal) {
        count += 1;
      }
      maxVal = std::max(node->val, maxVal);
      if (node->left != nullptr) {
        nodeQueue.emplace_back(node->left, maxVal);
      }
      if (node->right != nullptr) {
        nodeQueue.emplace_back(node->right, maxVal);
      }
    }
    return count;
  }
};

using std::array;
using std::nullopt;
using std::optional;

TEST(GoodNodesTest, SingleNodeTree) {
  array<optional<int>, 1> data{5};
  Tree<1> tree(data);
  EXPECT_EQ(Solution().goodNodes(tree.head()), 1);
}

TEST(GoodNodesTest, AllNodesAreGood) {
  // Tree structure:
  //       0
  //      / \
  //     1   4
  //    / \
  //   3   1
  array<optional<int>, 7> data{0, 1, 3, 1, 4, {}, {}};
  Tree<7> tree(data);
  EXPECT_EQ(Solution().goodNodes(tree.head()), 5);
}

TEST(GoodNodesTest, SomeNodesAreGood) {
  // Tree structure:
  //       3
  //      / \
  //     1   4
  //    /   / \
  //   3   1   5
  array<optional<int>, 7> data{3, 1, 4, 3, nullopt, 1, 5};
  Tree<7> tree(data);
  EXPECT_EQ(Solution().goodNodes(tree.head()), 4);
}

TEST(GoodNodesTest, AllNodesOnRightAreGood) {
  // Tree structure:
  //       2
  //        \
  //         4
  //        / \
  //       4   5
  array<optional<int>, 7> data{2, nullopt, 4, nullopt, nullopt, 4, 5};
  Tree<7> tree(data);
  EXPECT_EQ(Solution().goodNodes(tree.head()), 4);
}

TEST(GoodNodesTest, ComplexTree) {
  // Tree structure:
  //         10
  //        /  \
  //       5    15
  //      / \   / \
  //     2  8  12 20
  //    / \  \    /
  //   1  13  9   19
  array<optional<int>, 15> data{10, 5,       15,      2,       8,
                                12, 20,      1,       13,      nullopt,
                                9,  nullopt, nullopt, nullopt, 19};
  Tree<15> tree(data);
  EXPECT_EQ(Solution().goodNodes(tree.head()), 4);
}

TEST(GoodNodesTest, AllNodesSameValue) {
  // Tree structure:
  //       2
  //      / \
  //     2   2
  //    / \
  //   2   2
  array<optional<int>, 7> data{2, 2, 2, 2, 2};
  Tree<7> tree(data);
  EXPECT_EQ(Solution().goodNodes(tree.head()), 5);
}

TEST(GoodNodesTest, NegativeValues) {
  // Tree structure:
  //       -1
  //       / \
  //     -2 -3
  //     / \
  //   -4 -1
  array<optional<int>, 7> data{-1, -2, -3, -4, -1};
  Tree<7> tree(data);
  EXPECT_EQ(Solution().goodNodes(tree.head()), 2);
}
