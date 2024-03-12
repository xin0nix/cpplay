#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>
using namespace std;
using namespace testing;
// Definition for singly-linked list.
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

class Solution {
public:
  void reorderList(ListNode *head) {
    ListNode *l = head;
    ListNode *r = nullptr;
    ListNode base;
    ListNode *h = &base;
    while (l) {
      h->next = l;
      h = h->next;
      if (l->next == r) [[unlikely]]
        break;
      l = l->next;
      ListNode *k = l;
      while (k != r && k->next != r) [[likely]]
        k = k->next;
      h->next = k;
      h = h->next;
      r = k;
    }
    h->next = nullptr;
  }
};

TEST(ReorderListTest, LeetCodeExample1) {
  ListWrapper lw({1, 2, 3, 4});
  Solution().reorderList(lw.getHead());
  auto res = buildVec(lw.getHead());
  EXPECT_THAT(res, ElementsAre(1, 4, 2, 3));
}

TEST(ReorderListTest, LeetCodeExample2) {
  ListWrapper lw({1, 2, 3, 4, 5});
  Solution().reorderList(lw.getHead());
  auto res = buildVec(lw.getHead());
  EXPECT_THAT(res, ElementsAre(1, 5, 2, 4, 3));
}

TEST(ReorderListTest, Empty) {
  ListWrapper lw({});
  Solution().reorderList(lw.getHead());
  auto res = buildVec(lw.getHead());
  EXPECT_THAT(res, ElementsAre());
}

TEST(ReorderListTest, Single) {
  ListWrapper lw({1});
  Solution().reorderList(lw.getHead());
  auto res = buildVec(lw.getHead());
  EXPECT_THAT(res, ElementsAre(1));
}

// int main() {
//   ListWrapper lw({1, 2, 3});
//   Solution().reorderList(lw.getHead());
//   auto res = buildVec(lw.getHead());
//   for (auto r : res)
//     cout << r << ", ";
//   cout << endl;
// }
