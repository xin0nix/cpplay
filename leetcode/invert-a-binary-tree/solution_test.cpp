#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

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

constexpr bool isValidTreeSize(size_t s) {
  // Check if s is of the form 2^N - 1
  size_t lowerMask = s - 1;
  size_t upperMask = s + 1;
  return (s & lowerMask) == lowerMask and (s & upperMask) == 0;
}

template <size_t N>
  requires(isValidTreeSize(N)) // that's not enough actually
struct Tree {
  explicit Tree(const std::array<int, N> &vals) {
    std::transform(vals.cbegin(), vals.cend(), data.begin(), data.begin(),
                   [](int x, TreeNode &node) -> TreeNode {
                     node.val = x;
                     return node;
                   });
    // parent:     (i - 1) / 2
    // left child:  p * 2 + 1
    // right child: p * 2 + 2
    for (int p = 0; p <= N / 2; ++p) {
      const int l = p * 2 + 1;
      const int r = p * 2 + 2;
      data[p].left = &data[l];
      data[p].right = &data[r];
    }
  }

  std::array<TreeNode, N> data;

  friend std::ostream &operator<<(std::ostream &s, Tree &tree) {
    auto &data = tree.data;
    s << "(";
    for (auto it = data.begin(), e = data.end(); it != e; ++it) {
      s << it->val;
      if (it + 1 != data.end())
        s << ", ";
    }
    s << ")";
    return s;
  }
};

class Solution {
public:
  TreeNode *invertTree(TreeNode *root) {}
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
  std::array<int, 1> data{-1};
  Tree<1> tree(data);
  std::stringstream ss;
  ss << tree;
  ASSERT_EQ(ss.str(), "(-1)");
}

TEST(TreeHelperTest, Values0throuth2) {
  std::array<int, 3> data{0, 1, 2};
  Tree<3> tree(data);
  std::stringstream ss;
  ss << tree;
  ASSERT_EQ(ss.str(), "(0, 1, 2)");
}

