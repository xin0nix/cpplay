#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

using namespace std;

struct ListNode {
  int val;
  ListNode *next{nullptr};
  ListNode(int x) : val(x) {}
};

struct ListWrapper final {
  // dummy head that points to the beginning of the list
  vector<ListNode> list;
  // create list using initializer and index
  ListWrapper(initializer_list<int> vals, int index) {
    list.reserve(vals.size() + 1);
    list.emplace_back(0);
    for (auto v : vals) {
      ListNode &tail = list.back();
      list.emplace_back(v);
      tail.next = &list.back();
    }
    if (index < 0)
      return;                          // no loops, early exit
    ListNode *loop = &list[index + 1]; // take dummy head into account
    list.back().next = loop;
  }

  [[nodiscard]] ListNode *getHead() const { return list.front().next; }
};

class BruteForce {
public:
  bool hasCycle(ListNode *head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
      fast = fast->next->next; // double stride
      slow = slow->next;
      if (fast == slow)
        return true;
    }
    return false;
  }
};

TEST(LinkedListCycleTest, LeetCodeExample1) {
  ListWrapper lw({3, 2, 0, -4}, 1);
  EXPECT_TRUE(BruteForce().hasCycle(lw.getHead()));
}

TEST(LinkedListCycleTest, LeetCodeExample2) {
  ListWrapper lw({1, 2}, 0);
  EXPECT_TRUE(BruteForce().hasCycle(lw.getHead()));
}

TEST(LinkedListCycleTest, MyTest1) {
  ListWrapper lw({1, 2, 3, 4}, -1);
  EXPECT_FALSE(BruteForce().hasCycle(lw.getHead()));
}
