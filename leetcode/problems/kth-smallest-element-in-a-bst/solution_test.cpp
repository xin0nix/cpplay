#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>

// Структура узла дерева с сырыми указателями
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *l, TreeNode *r) : val(x), left(l), right(r) {}
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

// Класс решения с поиском k-го наименьшего элемента
class Solution {
public:
  int kthSmallest(TreeNode *root, int k) {
    counter = k;
    result = -1; // Значение по умолчанию, если k превышает количество узлов
    inorder(root);
    return result;
  }

private:
  int counter;
  int result;

  void inorder(TreeNode *node) {
    if (!node)
      return;

    inorder(node->left);

    if (--counter == 0) {
      result = node->val;
      return;
    }

    inorder(node->right);
  }
};

// Хелпер для создания дерева с сырыми указателями
TreeNode *makeNode(int val, TreeNode *left = nullptr,
                   TreeNode *right = nullptr) {
  return new TreeNode(val, left, right);
}

// Функция для удаления дерева и освобождения памяти
void deleteTree(TreeNode *root) {
  if (!root)
    return;
  deleteTree(root->left);
  deleteTree(root->right);
  delete root;
}

// Структура для параметров теста с описанием
struct TestCase {
  TreeNode *root;
  int k;
  int expected;
  std::string description;
};

// Тесты с использованием Google Test
class KthSmallestTest : public ::testing::TestWithParam<TestCase> {
protected:
  Solution sol;
};

TEST_P(KthSmallestTest, ReturnsCorrectKthSmallest) {
  const TestCase &param = GetParam();
  int result = sol.kthSmallest(param.root, param.k);
  std::ostringstream oss;
  oss << param.description << '\n';
  printTree(param.root, oss);
  EXPECT_EQ(result, param.expected) << oss.str();
  delete (param.root);
}

// Создаём деревья для тестов

// Дерево 1:
//       3
//      / \
//     1   4
//      \
//       2
TreeNode *tree1() {
  return makeNode(3, makeNode(1, nullptr, makeNode(2)), makeNode(4));
}

// Дерево 2:
//        5
//       / \
//      3   6
//     / \
//    2   4
//   /
//  1
TreeNode *tree2() {
  return makeNode(5,
                  makeNode(3, makeNode(2, makeNode(1), nullptr), makeNode(4)),
                  makeNode(6));
}

// Дерево 3 (правостороннее):
// 1
//  \
//   2
//    \
//     3
//      \
//       4
TreeNode *tree3() {
  return makeNode(1, nullptr,
                  makeNode(2, nullptr, makeNode(3, nullptr, makeNode(4))));
}

// Дерево 4 (левостороннее):
//       4
//      /
//     3
//    /
//   2
//  /
// 1
TreeNode *tree4() {
  return makeNode(4, makeNode(3, makeNode(2, makeNode(1), nullptr), nullptr),
                  nullptr);
}

// Параметры тестов
INSTANTIATE_TEST_SUITE_P(
    KthSmallestTests, KthSmallestTest,
    ::testing::Values(TestCase{tree1(), 1, 1, "Tree1: k=1, smallest element"},
                      TestCase{tree1(), 3, 3, "Tree1: k=3, middle element"},
                      TestCase{tree1(), 4, 4, "Tree1: k=4, largest element"},
                      TestCase{tree2(), 1, 1, "Tree2: k=1, smallest element"},
                      TestCase{tree2(), 5, 5, "Tree2: k=5, near largest"},
                      TestCase{tree2(), 6, 6, "Tree2: k=6, largest element"},
                      TestCase{tree3(), 2, 2, "Tree3: right skewed, k=2"},
                      TestCase{tree3(), 4, 4, "Tree3: right skewed, k=4"},
                      TestCase{tree4(), 1, 1, "Tree4: left skewed, k=1"},
                      TestCase{tree4(), 3, 3, "Tree4: left skewed, k=3"}));
