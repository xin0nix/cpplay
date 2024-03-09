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

class Solution {
public:
  bool hasCycle(ListNode *head) {
    if (!head)
      return false;
    ListNode *slow = head, *fast = head->next;
    while (fast) {
      for (int i = 0; fast && i < 2; fast = fast->next, ++i)
        if (fast == slow)
          return true;
      if (slow)
        slow = slow->next;
    }
    return false;
  }
};

TEST(LinkedListCycleTest, LeetCodeExample1) {
  ListWrapper lw({3, 2, 0, -4}, 1);
  EXPECT_TRUE(Solution().hasCycle(lw.getHead()));
}

TEST(LinkedListCycleTest, LeetCodeExample2) {
  ListWrapper lw({1, 2}, 0);
  EXPECT_TRUE(Solution().hasCycle(lw.getHead()));
}

TEST(LinkedListCycleTest, MyTest1) {
  ListWrapper lw({1, 2, 3, 4}, -1);
  EXPECT_FALSE(Solution().hasCycle(lw.getHead()));
}
