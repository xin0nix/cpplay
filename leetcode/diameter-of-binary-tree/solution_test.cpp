#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <type_traits>

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

#if __has_include(<concepts>) && __cpp_concepts >= 201907L
template <size_t N>
  requires(isValidTreeSize(N)) // that's not enough actually
#else
template <size_t N, typename = std::enable_if_t<isValidTreeSize(N)>>
#endif
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
};

std::ostream &operator<<(std::ostream &os, const TreeNode &root) {
  // parent:     (i - 1) / 2
  // left child:  p * 2 + 1
  // right child: p * 2 + 2
  std::deque<std::pair<const TreeNode *, int>> queue{{&root, 0}};
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

struct Solution {
  int diameterOfBinaryTree(TreeNode *root) {
    if (not root)
      return 0;
    struct DH {
      DH() : diameter(-1), height(-1) {}
      DH(int leftHeight, int rightHeight) {
        diameter = leftHeight + rightHeight + 2;
        height = std::max(leftHeight, rightHeight) + 1;
      }
      int diameter;
      int height;
    };
    struct {
      int maxDiameter = 0;
      DH diameterOfBinaryTree(const TreeNode *node) {
        if (!node)
          return {};
        auto leftDH = diameterOfBinaryTree(node->left);
        auto rightDH = diameterOfBinaryTree(node->right);
        DH result(leftDH.height, rightDH.height);
        maxDiameter = std::max(maxDiameter, result.diameter);
        return std::move(result);
      }
    } impl;
    std::ignore = impl.diameterOfBinaryTree(root);
    return impl.maxDiameter;
  }
};

struct SolutionWithStack {
  int diameterOfBinaryTree(TreeNode *root) {
    // TODO:
    // - Basic stack solution
    // - Advanced Morris Traversal
    return -1;
  }
};

#if __has_include(<concepts>) && __cpp_concepts >= 201907L

template <typename T>
concept IsSolutionType = requires(T t, TreeNode *root) {
  { t.diameterOfBinaryTree(root) } -> std::same_as<int>;
};

template <IsSolutionType T> struct DiameterOfBinTreeTestImpl {
  T solution;
};

#else

template <typename T, typename = void>
struct IsSolutionType : std::false_type {};

template <typename T>
struct IsSolutionType<
    T,
    std::void_t<std::is_same<decltype(std::declval<T &>().diameterOfBinaryTree(
                                 std::declval<TreeNode *>())),
                             int>>> : std::true_type {};

template <typename T, typename Enable = void> struct DiameterOfBinTreeTestImpl;

template <typename T>
struct DiameterOfBinTreeTestImpl<T,
                                 std::enable_if_t<IsSolutionType<T>::value>> {
  T solution;
};

#endif

template <typename T> struct DiameterOfBinTreeTest : public ::testing::Test {
  DiameterOfBinTreeTestImpl<T> impl;
  T &get() { return impl.solution; }
};

using TestTypes = ::testing::Types<Solution, SolutionWithStack>;
TYPED_TEST_SUITE(DiameterOfBinTreeTest, TestTypes);

TYPED_TEST(DiameterOfBinTreeTest, Basic1) {
  std::array<std::optional<int>, 15> data{1, 2, {}, 3, 4, {}, {}, 5, {}, {}, 6};
  Tree<15> tree(data);
  ASSERT_EQ(this->get().diameterOfBinaryTree(tree.head()), 4);
}
