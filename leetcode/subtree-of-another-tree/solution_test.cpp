#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
#include <iostream>
#include <queue>

#include <gtest/gtest.h>

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

namespace {
std::ostream &operator<<(std::ostream &os, TreeNode *data) {
  // parent:     (i - 1) / 2
  // left child:  p * 2 + 1
  // right child: p * 2 + 2
  std::deque<std::pair<TreeNode *, int>> queue{{data, 0}};
  os << "\n";
  while (!queue.empty()) {
    auto [node, indent] = queue.back();
    queue.pop_back();
    os << "|_" << std::string(indent, '_');
    if (!node) {
      os << "#\n";
      continue;
    }
    os << node->val;
    if (node->left || node->right) {
      queue.emplace_back(node->left, indent + 1);
      queue.emplace_back(node->right, indent + 1);
    }
    os << "\n";
  }
  return os;
}

constexpr bool isValidTreeSize(size_t s) {
  // Check if s is of the form 2^N - 1
  size_t lowerMask = s - 1;
  size_t upperMask = s + 1;
  return (s & lowerMask) == lowerMask and (s & upperMask) == 0;
}

template <size_t N>
  requires(isValidTreeSize(N)) // that's not enough actually
class Tree {
  std::array<TreeNode, N> data;

public:
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

  friend std::ostream &operator<<(std::ostream &os, Tree &tree) {
    os << tree.head();
    return os;
  }
};
} // namespace

struct Solution {
  [[nodiscard]] bool isSameTree(const TreeNode *left,
                                const TreeNode *right) const {
    if (not left and not right)
      return true;
    if (not left or not right)
      return false;
    if (left->val != right->val)
      return false;
    return isSameTree(left->left, right->left) and
           isSameTree(left->right, right->right);
  }

  [[nodiscard]] bool isSubtree(TreeNode *root, TreeNode *subRoot) {
    std::queue<const TreeNode *> visited;
    visited.push(root);
    while (not visited.empty()) {
      const TreeNode *top = visited.front();
      visited.pop();
      if (isSameTree(top, subRoot))
        return true;
      if (auto *l = top->left; l)
        visited.push(l);
      if (auto *r = top->right; r)
        visited.push(r);
    }
    return false;
  }
};

TEST(TreeHelperTest, TreeValidation) {
  for (size_t n = 2; n <= 4096; ++n) {
    if (std::ceil(std::log2(n)) == std::floor(std::log2(n))) {
      ASSERT_TRUE(isValidTreeSize(n - 1));
    } else {
      ASSERT_FALSE(isValidTreeSize(n - 1));
    }
  }
}

TEST(TreeHelperTest, SingleItem) {
  std::array<std::optional<int>, 1> data{-1};
  Tree<1> tree(data);
  std::stringstream ss;
  ss << tree;
  ASSERT_EQ(ss.str(), R"(
|_-1
)");
}

TEST(TreeHelperTest, Values0throuth2) {
  std::array<std::optional<int>, 7> data{0, 1, 2, {}, 4, 5, {}};
  Tree<7> tree(data);
  std::stringstream ss;
  ss << tree;
  std::string_view expected = R"(
|_0
|__2
|___#
|___5
|__1
|___4
|___#
)";
  ASSERT_EQ(ss.str(), expected);
}

TEST(SubtreeOfAnotherTreeTest, Leet1) {
  std::array<std::optional<int>, 7> left{3, 4, 5, 1, 2};
  std::array<std::optional<int>, 3> right{4, 1, 2};
  Tree<left.size()> leftTree(left);
  Tree<right.size()> rightTree(right);
  ASSERT_TRUE(Solution().isSubtree(leftTree.head(), rightTree.head()));
}

TEST(SubtreeOfAnotherTreeTest, Leet2) {
  std::array<std::optional<int>, 15> left{3, 4, 5, 1, 2, {}, {}, {}, {}, 0};
  std::array<std::optional<int>, 3> right{4, 1, 2};
  Tree<left.size()> leftTree(left);
  Tree<right.size()> rightTree(right);
  ASSERT_FALSE(Solution().isSubtree(leftTree.head(), rightTree.head()));
}
