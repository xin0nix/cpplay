#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

using namespace std;
using testing::ElementsAre;

namespace {
struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
  static void deallocate(ListNode *head) {
    while (head) {
      ListNode *next = head->next;
      delete head;
      head = next;
    }
  }
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
} // namespace

class BruteForce {
public:
  ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    int carry = 0;
    ListNode base;
    ListNode *tail = &base;
    while (l1 || l2) {
      int lhs = l1 ? l1->val : 0;
      int rhs = l2 ? l2->val : 0;
      int sum = carry + lhs + rhs;
      carry = sum / 10;
      tail->next = new ListNode(sum % 10);
      tail = tail->next;
      if (l1)
        l1 = l1->next;
      if (l2)
        l2 = l2->next;
    }
    if (carry)
      tail->next = new ListNode(carry);
    return base.next;
  }
};

TEST(AddTwoNumsTest, LeetCode1) {
  ListWrapper lhs({9, 9, 9, 9, 9, 9, 9});
  ListWrapper rhs({9, 9, 9, 9});
  ListNode *resHead = BruteForce().addTwoNumbers(lhs.getHead(), rhs.getHead());
  auto res = buildVec(resHead);
  EXPECT_THAT(res, ElementsAre(8, 9, 9, 9, 0, 0, 0, 1));
  ListNode::deallocate(resHead);
}

TEST(AddTwoNumsTest, NeetCode1) {
  ListWrapper lhs({2, 4, 3, 3});
  ListWrapper rhs({5, 6, 4});
  ListNode *resHead = BruteForce().addTwoNumbers(lhs.getHead(), rhs.getHead());
  auto res = buildVec(resHead);
  EXPECT_THAT(res, ElementsAre(7, 0, 8, 3));
  ListNode::deallocate(resHead);
}

TEST(AddTwoNumsTest, NeetCode2) {
  ListWrapper lhs({7});
  ListWrapper rhs({8});
  ListNode *resHead = BruteForce().addTwoNumbers(lhs.getHead(), rhs.getHead());
  auto res = buildVec(resHead);
  EXPECT_THAT(res, ElementsAre(5, 1));
  ListNode::deallocate(resHead);
}
