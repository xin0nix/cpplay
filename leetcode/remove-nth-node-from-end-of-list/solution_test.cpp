#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct ListWrapper final {
  // dummy head that points to the beginning of the list
  vector<ListNode> list;
  // create list using initializer and index
  ListWrapper(initializer_list<int> vals) {
    list.reserve(vals.size() + 1);
    list.emplace_back(0);
    for (auto v : vals) {
      ListNode &tail = list.back();
      list.emplace_back(v);
      tail.next = &list.back();
    }
  }

  [[nodiscard]] ListNode *getHead() const { return list.front().next; }
};

vector<int> buildVec(ListNode *head) {
  vector<int> result;
  if (!head)
    return {};
  while (head) {
    result.push_back(head->val);
    head = head->next;
  }
  return result;
}

// Given the head of a linked list, remove the nth node from the end of the list
// and return its head.
class Solution {
public:
  ListNode *removeNthFromEnd(ListNode *head, const int n) {}
};

TEST(RemoveNthTest, LeetCode1) {
  ListWrapper lw({1, 2, 3, 4, 5});
  ListNode *h = Solution().removeNthFromEnd(lw.getHead(), 2);
  auto res = buildVec(h);
  ASSERT_THAT(res, ::testing::ElementsAre(1, 2, 3, 5));
}

TEST(RemoveNthTest, LeetCode2) {
  ListWrapper lw({1});
  ListNode *h = Solution().removeNthFromEnd(lw.getHead(), 1);
  auto res = buildVec(h);
  ASSERT_THAT(res, ::testing::ElementsAre());
}

TEST(RemoveNthTest, LeetCode3) {
  ListWrapper lw({1, 2});
  ListNode *h = Solution().removeNthFromEnd(lw.getHead(), 1);
  auto res = buildVec(h);
  ASSERT_THAT(res, ::testing::ElementsAre(1));
}
