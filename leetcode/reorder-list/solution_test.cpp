#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
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
  void print() {
    ListNode *head = this;
    while (head) {
      cout << head->val << " ";
      head = head->next;
    }
    cout << std::endl;
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

struct CustomSolutionN2 {
  void reorderList(ListNode *head) {
    ListNode *l = head;
    ListNode *r = nullptr;
    ListNode dummy;
    ListNode *h = &dummy;
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

struct LinearSolution {
  ListNode *findMiddle(ListNode *head) {
    ListNode *slow = head, *fast = head->next;
    while (fast && fast->next) {
      slow = slow->next;
      fast = fast->next->next;
    }
    return slow;
  }

  ListNode *reverse(ListNode *middle) {
    ListNode *prev = middle, *cur = middle->next;
    prev->next = nullptr;
    while (cur) {
      ListNode *next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }
    return prev;
  }

  void merge(ListNode *head, ListNode *tail) {
    // Both lists share pointer to the last element, but it should only be
    // included once
    //
  }

  void reorderList(ListNode *head) {
    if (!head)
      return;
    ListNode *middle = findMiddle(head);
    ListNode *tail = reverse(middle);
    merge(head, tail);
  }
};

template <typename T> struct ReorderListTest : testing::Test {
  T solution;
  T &getSolution() { return solution; }
};

// using TestTypes = testing::Types<LinearSolution, CustomSolutionN2>;
using TestTypes = testing::Types<LinearSolution>;
TYPED_TEST_SUITE(ReorderListTest, TestTypes);

TYPED_TEST(ReorderListTest, LeetCodeExample1) {
  ListWrapper lw({1, 2, 3, 4});
  this->getSolution().reorderList(lw.getHead());
  auto res = buildVec(lw.getHead());
  EXPECT_THAT(res, ElementsAre(1, 4, 2, 3));
}

TYPED_TEST(ReorderListTest, LeetCodeExample2) {
  ListWrapper lw({1, 2, 3, 4, 5});
  this->getSolution().reorderList(lw.getHead());
  auto res = buildVec(lw.getHead());
  EXPECT_THAT(res, ElementsAre(1, 5, 2, 4, 3));
}

TYPED_TEST(ReorderListTest, Empty) {
  ListWrapper lw({});
  this->getSolution().reorderList(lw.getHead());
  auto res = buildVec(lw.getHead());
  EXPECT_THAT(res, ElementsAre());
}

TYPED_TEST(ReorderListTest, Single) {
  ListWrapper lw({1});
  this->getSolution().reorderList(lw.getHead());
  auto res = buildVec(lw.getHead());
  EXPECT_THAT(res, ElementsAre(1));
}

TYPED_TEST(ReorderListTest, Pair) {
  ListWrapper lw({1, 2});
  this->getSolution().reorderList(lw.getHead());
  auto res = buildVec(lw.getHead());
  EXPECT_THAT(res, ElementsAre(1, 2));
}

TYPED_TEST(ReorderListTest, Triple) {
  ListWrapper lw({1, 2, 3});
  this->getSolution().reorderList(lw.getHead());
  auto res = buildVec(lw.getHead());
  EXPECT_THAT(res, ElementsAre(1, 3, 2));
}
