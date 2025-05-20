#include <gtest/gtest.h>
#include <optional>
#include <queue>
#include <span>
#include <sstream>
#include <vector>

// Используем optional для обозначения null
using OptInt = std::optional<int>;

struct TestCase {
  std::vector<int> preorder;
  std::vector<int> inorder;
  std::vector<OptInt> expected_level_order; // nullopt для отсутствующих узлов
};

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

namespace {
std::ostream &operator<<(std::ostream &outStream,
                         std::span<const int> numbers) {
  outStream << "|";
  for (int num : numbers) {
    outStream << num << "|";
  }
  return outStream;
}
} // namespace

#include <ranges>
class Solution {
public:
  TreeNode *buildTree(const std::vector<int> &preorder,
                      const std::vector<int> &inorder) {
    struct Impl {
      TreeNode *operator()(std::span<const int> preOrder,
                           std::span<const int> inOrder) {
        if (preOrder.empty()) {
          return nullptr;
        }
        int root = preOrder.front();
        int leftCount = (int)std::distance(inOrder.begin(),
                                           std::ranges::find(inOrder, root));
        TreeNode *node = new TreeNode(root);
        node->left = operator()(preOrder.subspan(1, leftCount),
                                inOrder.subspan(0, leftCount));
        node->right = operator()(preOrder.subspan(1 + leftCount),
                                 inOrder.subspan(1 + leftCount));
        return node;
      }
    } impl;
    return impl(preorder, inorder);
  }
};

// Функция для вывода дерева в псевдографике (inorder с отступами)
void printTree(const TreeNode *node, std::ostream &os, std::string prefix = "",
               bool isLeft = true) {
  if (node == nullptr) {
    return;
  }
  printTree(node->right, os, prefix + (isLeft ? "│   " : "    "), false);
  os << prefix << (isLeft ? "└── " : "┌── ") << node->val << "\n";
  printTree(node->left, os, prefix + (isLeft ? "    " : "│   "), true);
}

// Функция сравнения дерева с expected_level_order с использованием
// optional<int>
void CompareTrees(TreeNode *root, const std::vector<OptInt> &expected) {
  if (root == nullptr and expected.empty()) {
    return;
  }
  std::queue<TreeNode *> queue;
  queue.push(root);
  std::stringstream oss;
  printTree(root, oss);
  size_t iter = 0;
  while (!queue.empty() and iter < expected.size()) {
    TreeNode *node = queue.front();
    queue.pop();
    if (node != nullptr) {
      if (!expected[iter].has_value() || node->val != expected[iter].value()) {
        FAIL() << "Node exists in the tree, but it does not match "
                  "expectations, nodeVal="
               << node->val << ", tree: \n"
               << oss.str();
        return;
      }
      queue.push(node->left);
      queue.push(node->right);
    } else {
      if (expected[iter].has_value()) {
        FAIL() << "Node does not exist in the tree, value="
               << expected[iter].value() << ", tree: \n"
               << oss.str();
        return;
      }
    }
    ++iter;
  }
  while (iter < expected.size()) {
    if (expected[iter].has_value()) {
      FAIL() << "Expectations exceeded result in size, value="
             << expected[iter].value() << ", tree: \n"
             << oss.str();
      return;
    }
    ++iter;
  }
}

class ConstructBinaryTreeTest : public ::testing::TestWithParam<TestCase> {
protected:
  TreeNode *BuildTreeWrapper(const std::vector<int> &preorder,
                             const std::vector<int> &inorder) {
    Solution sol;
    return sol.buildTree(preorder, inorder);
  }
};

void DeleteTree(TreeNode *root) {
  if (root == nullptr)
    return;
  DeleteTree(root->left);
  DeleteTree(root->right);
  delete root;
}

TEST_P(ConstructBinaryTreeTest, HandlesVariousInputs) {
  const TestCase &tc = GetParam();
  TreeNode *root = BuildTreeWrapper(tc.preorder, tc.inorder);
  CompareTrees(root, tc.expected_level_order);
  DeleteTree(root); // Очистка памяти после проверки
}

const std::vector<TestCase> gTestCases = {
    // Пример из LeetCode
    {{3, 9, 20, 15, 7},
     {9, 3, 15, 20, 7},
     {3, 9, 20, std::nullopt, std::nullopt, 15, 7}},
    // Пример из LeetCode с одним узлом
    {{-1}, {-1}, {-1}},
    // Все дочерние узлы левые
    {{4, 3, 2, 1}, {1, 2, 3, 4}, {4, 3, std::nullopt, 2, std::nullopt, 1}},
    // Все дочерние узлы правые
    {{1, 2, 3, 4},
     {1, 2, 3, 4},
     {1, std::nullopt, 2, std::nullopt, 3, std::nullopt, 4}},
    // Пустое дерево
    {{}, {}, {}},
    // Один узел
    {{1}, {1}, {1}}};

INSTANTIATE_TEST_SUITE_P(Default, ConstructBinaryTreeTest,
                         ::testing::ValuesIn(gTestCases));
