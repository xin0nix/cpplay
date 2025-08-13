import pytest
from typing import List, Optional
import functools


# Definition for singly-linked list.
class ListNode:
    def __init__(self, val=0, next: Optional["ListNode"] = None):
        self.val = val
        self.next = next

    @staticmethod
    def from_list(items: List[int]) -> Optional["ListNode"]:
        if not items:
            return None
        dummy = ListNode()
        current = dummy
        for item in items:
            current.next = ListNode(item)
            current = current.next
        return dummy.next

    @staticmethod
    def to_list(node: Optional["ListNode"]) -> List[int]:
        result = []
        current = node
        while current:
            result.append(current.val)
            current = current.next
        return result


class Solution:
    def mergeKLists(self, lists: List[Optional[ListNode]]) -> Optional[ListNode]:
        if len(lists) == 0:
            return None
        if len(lists) == 1:
            return lists[0]
        def mergeLists(a: Optional[ListNode], b: Optional[ListNode]) -> Optional[ListNode]:
            left = a
            right = b
            base = ListNode()
            tail = base
            while left and right:
                if left.val < right.val:
                    tail.next = left
                    left = left.next
                else:
                    tail.next = right
                    right = right.next
                tail = tail.next

            if left:
                tail.next = left
            else:
                tail.next = right

            return base.next
        return functools.reduce(mergeLists, lists)


@pytest.mark.parametrize(
    "input_lists, expected_output",
    [
        # Test case 1: Empty list of lists
        ([], None),
        # Test case 2: List of one empty list
        ([None], None),
        # Test case 3: List of one non-empty list
        ([ListNode.from_list([1, 4, 5])], ListNode.from_list([1, 4, 5])),
        # Test case 4: Multiple lists with sorted elements
        (
            [
                ListNode.from_list([1, 4, 5]),
                ListNode.from_list([1, 3, 4]),
                ListNode.from_list([2, 6]),
            ],
            ListNode.from_list([1, 1, 2, 3, 4, 4, 5, 6]),
        ),
        # Test case 5: Lists with some empty and some non-empty
        (
            [None, ListNode.from_list([2, 3, 8]), None, ListNode.from_list([1, 4])],
            ListNode.from_list([1, 2, 3, 4, 8]),
        ),
        # Test case 6: Lists with duplicates
        (
            [
                ListNode.from_list([1, 2, 2]),
                ListNode.from_list([1, 1, 2]),
                ListNode.from_list([2, 2, 3]),
            ],
            ListNode.from_list([1, 1, 1, 2, 2, 2, 2, 2, 3]),
        ),
    ],
)
def test_merge_k_lists(input_lists, expected_output):
    sol = Solution()
    result = sol.mergeKLists(input_lists)
    assert ListNode.to_list(result) == ListNode.to_list(expected_output)
