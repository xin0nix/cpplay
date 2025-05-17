#include <gtest/gtest.h>
#include <limits>

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
};

// Так как это задача на деревья думаю воспользоваться DFS через рекурсию. В
// каждом вызове будет передаваться интервал (min, max) текущего дерева (в
// начале (-Inf, +Inf)). Проверяем, попадает ли значения узла val в диапазон min
// < val < max? Если нет - конец. Если у дерева есть левая дочка, идём через
// неё, передавая (min, val). Если есть правая дочка - (val, max)

struct Solution {
  bool isValidBST(TreeNode *root) {
    struct Impl {
      bool operator()(int64_t lower, TreeNode *node, int64_t upper) {
        if (!node) {
          return true;
        }
        bool inRange = lower < node->val and node->val < upper;
        if (not inRange) {
          return false;
        }
        if (node->left != nullptr) {
          if (not operator()(lower, node->left, node->val)) {
            return false;
          }
        }
        if (node->right != nullptr) {
          if (not operator()(node->val, node->right, upper)) {
            return false;
          }
        }
        return true;
      }
    } impl{};
    return impl(std::numeric_limits<int64_t>::min(), root,
                std::numeric_limits<int64_t>::max());
  }
};

// Структура для хранения тестовых данных
struct TestCase {
  TreeNode *root;
  bool expected;
};

// Утилита для удаления дерева (чтобы избежать утечек памяти)
void deleteTree(TreeNode *root) {
  if (!root)
    return;
  deleteTree(root->left);
  deleteTree(root->right);
  delete root;
}

// Параметризованный тестовый класс
class ValidateBSTTest : public ::testing::TestWithParam<TestCase> {
protected:
  Solution sol;

  void TearDown() override {
    // Очистка памяти после каждого теста
    deleteTree(GetParam().root);
  }
};

TEST_P(ValidateBSTTest, HandlesVariousTrees) {
  const TestCase &param = GetParam();
  EXPECT_EQ(sol.isValidBST(param.root), param.expected);
}

// Функция для удобного создания узлов (чтобы код тестов был компактнее)
TreeNode *n(int val, TreeNode *left = nullptr, TreeNode *right = nullptr) {
  return new TreeNode(val, left, right);
}

// Инстанцирование параметров теста
INSTANTIATE_TEST_SUITE_P(
    BSTTests, ValidateBSTTest,
    ::testing::Values(
        // Пустое дерево
        TestCase{nullptr, true},

        // Один узел
        TestCase{n(10), true},
        TestCase{n(std::numeric_limits<int>::max()), true},
        TestCase{n(std::numeric_limits<int>::min()), true},

        // Корректное BST:
        TestCase{n(2, n(1), n(3)), true},

        // Некорректное BST: левый потомок больше родителя
        TestCase{n(5, n(6), n(7)), false},

        // Нарушение глубже в дереве
        TestCase{n(5, n(1), n(4, n(3), n(6))), false},

        // Корректное BST с отрицательными значениями
        TestCase{n(0, n(-3, n(-10), nullptr), n(9, n(5), nullptr)), true},

        // Повторяющиеся значения (невалидно)
        TestCase{n(2, n(2), n(3)), false}));