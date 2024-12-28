#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

using namespace std;

/** * Definition for singly-linked list. */
struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

vector<ListNode> buildList(vector<int> &&nums) {
  vector<ListNode> result;
  result.reserve(nums.size());
  for (auto it = nums.rbegin(); it != nums.rend(); ++it) {
    ListNode *prev = nullptr;
    if (!result.empty())
      prev = &result.back();
    result.emplace_back(*it, prev);
  }
  return result;
}

ListNode *getListHead(vector<ListNode> &list) {
  if (list.empty())
    return nullptr;
  return &list.back();
}

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

class BruteForce {
public:
  ListNode *reverseList(ListNode *head) {
    // loop invariant
    // - prev might be null
    // - head must not be null
    // - head->next might be null
    // - "head" on the "prev" iteration used to be "next" relative to it
    ListNode *prev = nullptr;
    while (head) {
      ListNode *next = head->next;
      head->next = prev; // prev
      prev = head;
      head = next;
    }
    return prev;
  }
};

TEST(ReverseLinkedListTest, CheckList) {
  auto list = buildList({1, 2, 3, 4, 5});
  auto re = buildVec(getListHead(list));
  ASSERT_THAT(re, testing::ElementsAre(1, 2, 3, 4, 5));
}

TEST(ReverseLinkedListTest, CheckEmptyList) {
  auto list = buildList({});
  auto re = buildVec(getListHead(list));
  ASSERT_THAT(re, testing::ElementsAre());
}

TEST(ReverseLinkedListTest, LeetCodeExample1) {
  auto list = buildList({1, 2, 3, 4, 5});
  auto *re = BruteForce().reverseList(getListHead(list));
  auto vec = buildVec(re);
  ASSERT_THAT(vec, testing::ElementsAre(5, 4, 3, 2, 1));
}

TEST(ReverseLinkedListTest, LeetCodeExample2) {
  auto list = buildList({1, 2});
  auto *re = BruteForce().reverseList(getListHead(list));
  auto vec = buildVec(re);
  ASSERT_THAT(vec, testing::ElementsAre(2, 1));
}

TEST(ReverseLinkedListTest, Empty) {
  auto list = buildList({});
  auto *re = BruteForce().reverseList(getListHead(list));
  auto vec = buildVec(re);
  ASSERT_THAT(vec, testing::ElementsAre());
}