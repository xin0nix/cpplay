import pytest
from typing import List
from collections import deque


class Node:
    def __init__(self, L, R, n):
        self.left = L
        self.right = R
        self.value = n


def tree_by_levels(node: "Node | None"):
    if not node:
        return []
    r = []
    q = deque([node])
    while q:
        cur = q.popleft()
        r.append(cur.value)
        if cur.left:
            q.append(cur.left)
        if cur.right:
            q.append(cur.right)
    return r


def test_tree_by_levels_empty():
    assert tree_by_levels(None) == []


def test_tree_by_levels_example1():
    # Tree:
    #       2
    #    8     9
    #  1  3   4  5
    n1 = Node(None, None, 1)
    n3 = Node(None, None, 3)
    n4 = Node(None, None, 4)
    n5 = Node(None, None, 5)
    n8 = Node(n1, n3, 8)
    n9 = Node(n4, n5, 9)
    root = Node(n8, n9, 2)
    assert tree_by_levels(root) == [2, 8, 9, 1, 3, 4, 5]


def test_tree_by_levels_example2():
    # Tree:
    #       1
    #    8     4
    #     3      5
    #             7
    n3 = Node(None, None, 3)
    n7 = Node(None, None, 7)
    n5 = Node(None, n7, 5)
    n8 = Node(None, n3, 8)
    n4 = Node(None, n5, 4)
    root = Node(n8, n4, 1)
    assert tree_by_levels(root) == [1, 8, 4, 3, 5, 7]


def test_tree_by_levels_single_node():
    root = Node(None, None, 10)
    assert tree_by_levels(root) == [10]


def test_tree_by_levels_unbalanced():
    # Tree:
    #       1
    #      /
    #     2
    #      \
    #       3
    n3 = Node(None, None, 3)
    n2 = Node(None, n3, 2)
    root = Node(n2, None, 1)
    assert tree_by_levels(root) == [1, 2, 3]
