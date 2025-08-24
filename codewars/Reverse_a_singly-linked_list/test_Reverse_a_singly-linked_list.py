import pytest


# Assuming Node and make_linked_list are defined like this for testing purpose:
class Node:
    def __init__(self, value, next=None):
        self.value = value
        self.next = next


def reverse_list(node: "Node | None") -> "Node | None":
    if not node:
        return None

    prev = None
    cur = node
    while cur.next:
        tmp = cur.next
        cur.next = prev
        prev = cur
        cur = tmp
    cur.next = prev
    return cur


def make_linked_list(values):
    if not values:
        return None
    head = Node(values[0])
    current = head
    for v in values[1:]:
        current.next = Node(v)
        current = current.next
    return head


def linked_list_to_list(node):
    result = []
    while node:
        result.append(node.value)
        node = node.next
    return result


def test_reverse_empty_list():
    assert reverse_list(None) is None


def test_reverse_single_element():
    node = make_linked_list([1])
    reversed_node = reverse_list(node)
    assert linked_list_to_list(reversed_node) == [1]


def test_reverse_multiple_elements():
    original = [1, 2, 3, 4, 5]
    node = make_linked_list(original)
    reversed_node = reverse_list(node)
    assert linked_list_to_list(reversed_node) == original[::-1]


def test_reverse_large_list():
    original = list(range(1000))  # large list
    node = make_linked_list(original)
    reversed_node = reverse_list(node)
    assert linked_list_to_list(reversed_node) == original[::-1]
