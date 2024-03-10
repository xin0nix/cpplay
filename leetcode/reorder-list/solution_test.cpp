#include <vector>
using namespace std;
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

class Solution {
public:
  void reorderList(ListNode *head) {
    ListNode *from = head;
    ListNode *to = nullptr;
    ListNode base;
    // TODO: implement solution using 2 pointers
  }
};
