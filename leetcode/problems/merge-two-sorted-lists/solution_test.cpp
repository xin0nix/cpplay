#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

using namespace std;
using namespace ::testing;

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

vector<ListNode> buildList(vector<int> &nums) {
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
  ListNode *mergeTwoLists(ListNode *a, ListNode *b) {
    ListNode head;
    ListNode *tail = &head;

    while (a && b) {
      if (a->val <= b->val) {
        tail->next = a;
        tail = a;
        a = a->next;
      } else {
        tail->next = b;
        tail = b;
        b = b->next;
      }
    }
    if (a)
      tail->next = a;
    else if (b)
      tail->next = b;
    return head.next;
  }
};

TEST(MergeTwoSortedListsTest, LeetCodeExample1) {
  vector a{1, 2, 4};
  vector b{1, 3, 4};
  auto aList = buildList(a);
  auto bList = buildList(b);
  auto *cList =
      BruteForce().mergeTwoLists(getListHead(aList), getListHead(bList));
  auto c = buildVec(cList);
  EXPECT_THAT(c, ElementsAre(1, 1, 2, 3, 4, 4));
}

TEST(MergeTwoSortedListsTest, ExtendedExample1) {
  vector a{1, 2, 3};
  vector b{1, 3, 4, 5, 6};
  auto aList = buildList(a);
  auto bList = buildList(b);
  auto *cList =
      BruteForce().mergeTwoLists(getListHead(aList), getListHead(bList));
  auto c = buildVec(cList);
  EXPECT_THAT(c, ElementsAre(1, 1, 2, 3, 3, 4, 5, 6));
}

TEST(MergeTwoSortedListsTest, EmptyExample) {
  vector<int> a{};
  vector<int> b{};
  auto aList = buildList(a);
  auto bList = buildList(b);
  auto *cList =
      BruteForce().mergeTwoLists(getListHead(aList), getListHead(bList));
  EXPECT_EQ(cList, nullptr);
}
