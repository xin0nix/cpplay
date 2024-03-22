#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

using namespace std;
using testing::ElementsAre;

// Definition for a Node.
class Node {
public:
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
          return static_cast<int>(std::distance(head, cur->random));
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
  EXPECT_THAT(vw.randomShuffle, testing::ElementsAre(-1, 0, 4, 2, 0));
}

class Solution {
public:
  Node *copyRandomList(Node *head) {
    // TODO:
  }
};
