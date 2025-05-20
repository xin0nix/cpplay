#include <gtest/gtest.h>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Node {
public:
  int val;
  vector<Node *> neighbors;
  // Node() : val(0), neighbors(vector<Node *>()) {}
  Node(int _val) : val(_val), neighbors(vector<Node *>()) {}
  Node(int _val, vector<Node *> _neighbors)
      : val(_val), neighbors(_neighbors) {}
};

struct Solution {
  Node *cloneGraph(Node *root) {
    if (root == nullptr) {
      return nullptr;
    }
    std::array<Node *, 101> original = {nullptr};
    std::array<Node *, 101> clones = {nullptr};
    std::queue<Node *> queue;
    queue.push(root);
    // 1. **Первый проход:** Создаём клоны всех узлов без связей, проходя по
    // графу (например, обходом в ширину или глубину). При этом для каждого
    // оригинального узла создаём соответствующий клон и сохраняем отображение:
    // оригинал → клон.
    while (not queue.empty()) {
      auto *node = queue.front();
      queue.pop();
      // Клон точно ещё не в мапе
      int idx = node->val;
      auto *nodeClone = new Node(idx);
      original[idx] = node;
      clones[idx] = nodeClone;
      for (auto *nbr : node->neighbors) {
        if (clones[nbr->val] != nullptr) {
          continue;
        }
        queue.push(nbr);
      }
    }
    // 2. **Второй проход:** Пробегаемся по всем узлам оригинального графа и
    // для каждого клона добавляем ссылки на клоны соседей, используя
    // отображение из первого прохода.
    for (int i = 0; i < 101; ++i) {
      auto *cloned = clones[i];
      if (!cloned) {
        continue;
      }
      auto *orig = original[i];
      for (auto *nbr : orig->neighbors) {
        cloned->neighbors.push_back(clones[nbr->val]);
      }
    }
    return clones[root->val];
  }
};

// Вспомогательные функции

struct Deleter {
  // Рекурсивное удаление графа (освобождение памяти)
  void operator()(Node *node) {
    if (node == nullptr or visited.count(node)) {
      return;
    }
    visited.insert(node);
    for (Node *neighbor : node->neighbors) {
      operator()(neighbor);
    }
    delete node;
  }

  set<Node *> visited;
};

// Рекурсивное удаление графа (освобождение памяти)

// Сравнение двух графов на изоморфизм по структуре и значениям
struct Comparator {
  bool operator()(Node *n1, Node *n2) {
    if (!n1 && !n2) {
      return true;
    }
    if (!n1 || !n2) {
      return false;
    }
    if (n1->val != n2->val) {
      return false;
    }
    if (visited.count(n1)) {
      return visited[n1] == n2;
    }
    if (n1->neighbors.size() != n2->neighbors.size()) {
      return false;
    }

    visited[n1] = n2;

    for (size_t i = 0; i < n1->neighbors.size(); ++i) {
      if (!operator()(n1->neighbors[i], n2->neighbors[i])) {
        return false;
      }
    }
    return true;
  }
  unordered_map<Node *, Node *> visited;
};

// Печать графа (обход в ширину) для отладки
string PrintGraph(Node *node) {
  if (!node)
    return "null";

  unordered_map<Node *, bool> visited;
  vector<Node *> queue{node};
  visited[node] = true;
  ostringstream oss;

  while (!queue.empty()) {
    Node *cur = queue.back();
    queue.pop_back();
    oss << "Node " << cur->val << ": [";
    for (size_t i = 0; i < cur->neighbors.size(); ++i) {
      oss << cur->neighbors[i]->val;
      if (i + 1 < cur->neighbors.size()) {
        oss << ", ";
      }
      if (!visited[cur->neighbors[i]]) {
        visited[cur->neighbors[i]] = true;
        queue.insert(queue.begin(), cur->neighbors[i]);
      }
    }
    oss << "]\n";
  }
  return oss.str();
}

// Структура для параметров теста
struct GraphTestCase {
  string name;
  Node *input;
  Node *expected;
};

// Тестовый фикстурный класс с параметрами
class CloneGraphTest : public ::testing::TestWithParam<GraphTestCase> {
protected:
  void TearDown() override {
    // Очистка памяти после каждого теста
    if (GetParam().input) {
      Deleter{}(GetParam().input);
    }
    if (GetParam().expected) {
      Deleter{}(GetParam().expected);
    }
    if (cloned_ != nullptr) {
      Deleter{}(cloned_);
    }
  }

  Node *cloned_ = nullptr;
};

// Здесь должна быть ваша функция клонирования графа
Node *cloneGraph(Node *node) { return Solution().cloneGraph(node); }

// Тест с параметрами
TEST_P(CloneGraphTest, CloneCorrectness) {
  const GraphTestCase &param = GetParam();

  cloned_ = cloneGraph(param.input);

  bool equal = Comparator{}(param.expected, cloned_);
  if (!equal) {
    // Если сравнение не удалось, выводим графы
    ADD_FAILURE() << "Test case: " << param.name << "\n"
                  << "Expected graph:\n"
                  << PrintGraph(param.expected) << "Cloned graph:\n"
                  << PrintGraph(cloned_);
  }
  EXPECT_TRUE(equal);
}

// Пример создания узлов для тестов (без циклов, с циклами, пустой граф и т.п.)
Node *CreateGraph1() {
  // Граф: 1 -- 2
  Node *n1 = new Node(1);
  Node *n2 = new Node(2);
  n1->neighbors.push_back(n2);
  n2->neighbors.push_back(n1);
  return n1;
}

Node *CreateGraph2() {
  // Граф: 1 -- 2 -- 3, 1 -- 3
  Node *n1 = new Node(1);
  Node *n2 = new Node(2);
  Node *n3 = new Node(3);
  n1->neighbors = {n2, n3};
  n2->neighbors = {n1, n3};
  n3->neighbors = {n1, n2};
  return n1;
}

Node *CreateGraphEmpty() { return nullptr; }

Node *CreateGraphSingleNode() { return new Node(1); }

// Инстанцирование тестов с разными сценариями
INSTANTIATE_TEST_SUITE_P(
    CloneGraphTests, CloneGraphTest,
    ::testing::Values(
        GraphTestCase{"Empty", CreateGraphEmpty(), CreateGraphEmpty()},
        GraphTestCase{"Single_node", CreateGraphSingleNode(),
                      CreateGraphSingleNode()},
        GraphTestCase{"Two_nodes_connected", CreateGraph1(), CreateGraph1()},
        GraphTestCase{"Triangle_graph", CreateGraph2(), CreateGraph2()}),
    [](const testing::TestParamInfo<CloneGraphTest::ParamType> &info) {
      return info.param.name;
    });
