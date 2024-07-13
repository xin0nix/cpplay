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

  TreeNode *get(const int val) {
    if (auto it = std::find_if(data.begin(), data.end(),
                               [val](TreeNode &n) { return n.val == val; });
        it != data.end()) {
      return it;
    }
    return nullptr;
  }

  friend std::ostream &operator<<(std::ostream &os, Tree &tree) {
    os << tree.head();
    return os;
  }
};
} // namespace

struct Solution {
  TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
    struct Impl {
      struct SG {
        std::reference_wrapper<Impl> i;
        ~SG() { i.get().reset(); }
      };
      const int pVal;
      const int qVal;
      bool seenP = false;
      bool seenQ = false;
      bool bothReachable() { return seenP and seenQ; }
      void reset() {
        seenP = false;
        seenQ = false;
      }
      Impl &visit(const TreeNode *node) {
        if (node->val == pVal)
          seenP = true;
        else if (node->val == qVal)
          seenQ = true;
        if (seenP and seenQ)
          return *this;
        if (auto *l = node->left; l)
          visit(l);
        if (auto *r = node->right; r)
          visit(r);
        return *this;
      }
    } impl{p->val, q->val};
    std::queue<TreeNode *> nodeQueue;
    nodeQueue.push(root);
    TreeNode *lcaNode = root;
    while (not nodeQueue.empty()) {
      Impl::SG scopeGuard{std::ref(impl)};
      auto *top = nodeQueue.front();
      nodeQueue.pop();
      if (not impl.visit(top).bothReachable())
        continue; // dead end
      lcaNode = top;
      if (auto *l = top->left; l)
        nodeQueue.push(l);
      if (auto *r = top->right; r)
        nodeQueue.push(r);
    }
    return lcaNode;
  }
};

TEST(LCATreeTest, TreeValidation) {
  for (size_t n = 2; n <= 4096; ++n) {
    if (std::ceil(std::log2(n)) == std::floor(std::log2(n))) {
      ASSERT_TRUE(isValidTreeSize(n - 1));
    } else {
      ASSERT_FALSE(isValidTreeSize(n - 1));
    }
  }
}

TEST(LCATreeTest, SingleItem) {
  std::array<std::optional<int>, 1> data{-1};
  Tree<1> tree(data);
  std::stringstream ss;
  ss << tree;
  ASSERT_EQ(ss.str(), R"(
|_-1
)");
}

TEST(LCATreeTest, Values0throuth2) {
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

TEST(LCATreeTest, Leet1) {
  std::array<std::optional<int>, 15> data{6, 2, 8, 0, 4, 7, 9, {}, {}, 3, 5};
  Tree<data.size()> tree(data);
  int pVal = 2;
  int qVal = 8;
  int lcaVal = 6;
  auto *p = tree.get(pVal);
  ASSERT_EQ(p->val, pVal);
  auto *q = tree.get(qVal);
  ASSERT_EQ(q->val, qVal);
  auto *lca = tree.get(lcaVal);
  ASSERT_EQ(lca->val, lcaVal);
  auto *res = Solution().lowestCommonAncestor(tree.head(), p, q);
  ASSERT_EQ(res->val, lca->val);
}

TEST(LCATreeTest, Leet2) {
  std::array<std::optional<int>, 15> data{6, 2, 8, 0, 4, 7, 9, {}, {}, 3, 5};
  Tree<data.size()> tree(data);
  int pVal = 2;
  int qVal = 4;
  int lcaVal = 2;
  auto *p = tree.get(pVal);
  ASSERT_EQ(p->val, pVal);
  auto *q = tree.get(qVal);
  ASSERT_EQ(q->val, qVal);
  auto *lca = tree.get(lcaVal);
  ASSERT_EQ(lca->val, lcaVal);
  auto *res = Solution().lowestCommonAncestor(tree.head(), p, q);
  ASSERT_EQ(res->val, lca->val);
}

TEST(LCATreeTest, Leet3) {
  std::array<std::optional<int>, 3> data{2, 1};
  Tree<data.size()> tree(data);
  int pVal = 2;
  int qVal = 1;
  int lcaVal = 2;
  auto *p = tree.get(pVal);
  ASSERT_EQ(p->val, pVal);
  auto *q = tree.get(qVal);
  ASSERT_EQ(q->val, qVal);
  auto *lca = tree.get(lcaVal);
  ASSERT_EQ(lca->val, lcaVal);
  auto *res = Solution().lowestCommonAncestor(tree.head(), p, q);
  ASSERT_EQ(res->val, lca->val);
}

TEST(LCATreeTest, Custom1) {
  std::array<std::optional<int>, 15> data{6, 2, 8, 0, 4, 7, 9, {}, {}, 3, 5};
  Tree<data.size()> tree(data);
  int pVal = 0;
  int qVal = 5;
  int lcaVal = 2;
  auto *p = tree.get(pVal);
  ASSERT_EQ(p->val, pVal);
  auto *q = tree.get(qVal);
  ASSERT_EQ(q->val, qVal);
  auto *lca = tree.get(lcaVal);
  ASSERT_EQ(lca->val, lcaVal);
  auto *res = Solution().lowestCommonAncestor(tree.head(), p, q);
  ASSERT_EQ(res->val, lca->val);
}

TEST(LCATreeTest, Custom2) {
  std::array<std::optional<int>, 15> data{6, 2, 8, 0, 4, 7, 9, {}, {}, 3, 5};
  Tree<data.size()> tree(data);
  int pVal = 7;
  int qVal = 9;
  int lcaVal = 8;
  auto *p = tree.get(pVal);
  ASSERT_EQ(p->val, pVal);
  auto *q = tree.get(qVal);
  ASSERT_EQ(q->val, qVal);
  auto *lca = tree.get(lcaVal);
  ASSERT_EQ(lca->val, lcaVal);
  auto *res = Solution().lowestCommonAncestor(tree.head(), p, q);
  ASSERT_EQ(res->val, lca->val);
}
