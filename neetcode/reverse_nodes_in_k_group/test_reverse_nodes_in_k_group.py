import pytest

from typing import Optional, List


class ListNode:
    def __init__(self, val=0, next: Optional["ListNode"] = None):
        self.val = val
        self.next = next


class Solution:
    def reverse(self, cur: Optional[ListNode], tail: Optional[ListNode]):
        prev = None
        while cur and cur != tail:
            temp = cur.next
            cur.next = prev
            prev = cur
            cur = temp
        if tail:
            tail.next = prev
        return tail

    def getKth(self, cur: Optional["ListNode"], k: int):
        if not cur:
            return None
        counter = 1
        while cur and counter < k:
            cur = cur.next
            counter += 1
        return cur

    def reverseKGroup(self, head: Optional[ListNode], k: int) -> Optional[ListNode]:
        if not head:
            return None
        dummy = ListNode(0)
        dummy.next = head
        prev_group = dummy
        while True:
            group_start = prev_group.next
            kth_node = self.getKth(group_start, k)
            if not kth_node:
                return dummy.next
            next_group_start = kth_node.next
            self.reverse(group_start, kth_node)

            prev_group.next = kth_node
            prev_group = group_start
            group_start.next = next_group_start


def linked_list_to_list(head: Optional[ListNode]):
    """Helper to convert linked list to Python list."""
    result = []
    while head:
        result.append(head.val)
        head = head.next
    return result


def list_to_linked_list(lst: List[int]) -> ListNode:
    """Helper to convert Python list to linked list."""
    dummy = ListNode()
    current = dummy
    for val in lst:
        current.next = ListNode(val)
        current = current.next
    return dummy.next


@pytest.mark.parametrize(
    "input_list, k, expected_list",
    [
        ([1, 2, 3, 4, 5], 2, [2, 1, 4, 3, 5]),
        ([1, 2, 3, 4, 5], 3, [3, 2, 1, 4, 5]),
        ([1, 2, 3, 4, 5], 1, [1, 2, 3, 4, 5]),
        ([1], 1, [1]),
        ([], 3, []),
        ([1, 2], 3, [1, 2]),
    ],
)
def test_reverseKGroup(input_list, k, expected_list):
    sol = Solution()
    head = list_to_linked_list(input_list)
    result_head = sol.reverseKGroup(head, k)
    result_list = linked_list_to_list(result_head)
    assert result_list == expected_list


def find_node_by_value(head: Optional[ListNode], value: int) -> Optional[ListNode]:
    current = head
    while current:
        if current.val == value:
            return current
        current = current.next
    return None


@pytest.mark.parametrize(
    "list_values, k, expected_val",
    [
        ([1, 2, 3, 4, 5], 3, 3),  # 3rd node value is 3
        ([10, 20, 30, 40], 1, 10),  # 1st node value is 10
        ([5, 6, 7], 5, None),  # k larger than length returns None
        ([], 1, None),  # empty list returns None
        ([7], 1, 7),  # single element list returns that element
    ],
)
def test_getKth(list_values, k, expected_val):
    sol = Solution()
    head = list_to_linked_list(list_values)
    kth_node = sol.getKth(head, k)

    if expected_val is None:
        assert kth_node is None
    else:
        assert kth_node is not None
        assert kth_node.val == expected_val


@pytest.mark.parametrize(
    "input_list, tail_val, expected_list",
    [
        ([1, 2, 3], 3, [3, 2, 1]),  # reverse up to node with val=3
        ([1, 2, 3, 4], 4, [4, 3, 2, 1]),
        ([1], 1, [1]),
        ([], 0, []),
        ([1, 2], 2, [2, 1]),
    ],
)
def test_reverse(input_list, tail_val, expected_list):
    sol = Solution()
    head = list_to_linked_list(input_list)
    tail = find_node_by_value(head, tail_val)
    reversed_head = sol.reverse(head, tail)
    as_list = linked_list_to_list(reversed_head)
    assert as_list == expected_list
