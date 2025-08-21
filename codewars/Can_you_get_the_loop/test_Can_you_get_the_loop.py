import pytest


class Node:
    def __init__(self, value: int):
        self.value = value
        self.next: Node


def loop_size(node: Node | None):
    if not node:
        return 0
    slow = node
    fast = node.next
    while slow != fast:
        slow = slow.next
        fast = fast.next.next
    size = 1
    fast = fast.next
    while slow != fast:
        size += 1
        fast = fast.next
    return size


def create_linked_list_with_loop(dangling_length, loop_length):
    # Create dangling part
    head = Node(0)
    current = head
    for i in range(1, dangling_length):
        current.next = Node(i)
        current = current.next

    # Create loop part
    loop_start = Node(dangling_length)
    current.next = loop_start
    current = loop_start
    for i in range(dangling_length + 1, dangling_length + loop_length):
        current.next = Node(i)
        current = current.next
    current.next = loop_start  # Close the loop

    return head


def create_linked_list_loop_only(loop_length):
    loop_start = Node(0)
    current = loop_start
    for i in range(1, loop_length):
        current.next = Node(i)
        current = current.next
    current.next = loop_start  # Close the loop
    return loop_start


@pytest.mark.parametrize(
    "dangling, loop_len",
    [
        (3, 12),  # dangling piece 3 nodes, loop length 12 nodes
        (0, 5),  # loop only, length 5
        (1, 1),  # 1 dangling, 1 loop node (loop size 1)
        (0, 1),  # loop only of size 1
    ],
)
def test_loop_size(dangling, loop_len):
    if dangling > 0:
        head = create_linked_list_with_loop(dangling, loop_len)
    else:
        head = create_linked_list_loop_only(loop_len)

    assert loop_size(head) == loop_len
