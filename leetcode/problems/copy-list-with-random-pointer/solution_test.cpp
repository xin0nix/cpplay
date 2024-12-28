#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <unordered_map>
#include <vector>

using namespace std;
using testing::ElementsAre;

// Definition for a Node.
struct Node {
  static void deallocate(Node *head) {
    while (head) {
      Node *next = head->next;
      delete head;
      head = next;
    }
  }

  int val;
  Node *next;
  Node *random;

  Node(int _val) {
    val = _val;
    next = nullptr;
    random = nullptr;
  }
};

struct ListWrapper final {
  // dummy head that points to the beginning of the list
  vector<Node> base;
  // create list using initializer and index
  ListWrapper(initializer_list<int> vals, initializer_list<int> randomShuffle) {
    base.reserve(vals.size() + 1);
    base.emplace_back(0);
    for (auto v : vals) {
      Node &tail = base.back();
      base.emplace_back(v);
      tail.next = &base.back();
    }
    Node *cur = base.front().next;
    for (auto rndIdx : randomShuffle) {
      if (rndIdx >= 0)
        cur->random = &base[rndIdx + 1];
      cur = cur->next;
    }
  }

  [[nodiscard]] Node *getHead() const { return base.front().next; }
};

struct VectorWrapper {
  std::vector<int> result;
  std::vector<int> randomShuffle;
  VectorWrapper(Node *head) {
    if (!head)
      return;
    Node *cur = head;
    while (cur) {
      result.push_back(cur->val);
      int idx = [&] {
        if (cur->random)
          return cur->random->val;
        return -1;
      }();
      randomShuffle.push_back(idx);
      cur = cur->next;
    }
  }
};

TEST(WrapperTest, Basic) {
  ListWrapper lw({7, 13, 11, 10, 1}, {-1, 0, 4, 2, 0});
  VectorWrapper vw(lw.getHead());
  EXPECT_THAT(vw.result, testing::ElementsAre(7, 13, 11, 10, 1));
  EXPECT_THAT(vw.randomShuffle, testing::ElementsAre(-1, 7, 1, 11, 7));
}

class BruteForce {
public:
  Node *copyRandomList(Node *head) {
    unordered_map<Node *, Node *> nodeMap;
    Node base(0);
    // 1st iteration: clone the list itself
    Node *clone = &base;
    for (Node *cur = head; cur; cur = cur->next) {
      Node *newNode = new Node(cur->val);
      nodeMap[cur] = newNode;
      clone->next = newNode;
      newNode->random = cur->random;
      clone = newNode;
    }
    // 2nd iteration: remap the random pointers
    for (Node *cur = base.next; cur; cur = cur->next) {
      if (!cur->random)
        continue;
      cur->random = nodeMap[cur->random];
    }
    return base.next;
  }
};

TEST(CopyListSolution, LeetCode1) {
  ListWrapper lw({7, 13, 11, 10, 1}, {-1, 0, 4, 2, 0});
  Node *head = BruteForce().copyRandomList(lw.getHead());
  VectorWrapper vw(head);
  EXPECT_THAT(vw.result, testing::ElementsAre(7, 13, 11, 10, 1));
  EXPECT_THAT(vw.randomShuffle, testing::ElementsAre(-1, 7, 1, 11, 7));
  Node::deallocate(head);
}
