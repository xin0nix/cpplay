import pytest

from typing import Optional


# Definition for a binary tree node.
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


class Solution:
    def __init__(self):
        self.res: int = -1000

    def dfs(self, node: Optional[TreeNode]) -> int:
        if not node:
            return 0
        left = self.dfs(node.left)
        right = self.dfs(node.right)
        full_tree = node.val + max(left, 0) + max(right, 0)
        self.res = max(self.res, full_tree)
        return node.val + max(left, right, 0)

    def maxPathSum(self, root: Optional[TreeNode]) -> int:
        self.dfs(root)
        return self.res


# Helper to build trees from list input (level order)
def build_tree(nodes):
    if not nodes:
        return None
    root = TreeNode(nodes[0])
    queue = [root]
    i = 1
    while queue and i < len(nodes):
        current = queue.pop(0)
        if nodes[i] is not None:
            current.left = TreeNode(nodes[i])
            queue.append(current.left)
        i += 1
        if i < len(nodes) and nodes[i] is not None:
            current.right = TreeNode(nodes[i])
            queue.append(current.right)
        i += 1
    return root


@pytest.mark.parametrize(
    "tree_list, expected",
    [
        # Simple case: single node
        ([1], 1),
        # Regular tree with positive numbers
        ([1, 2, 3], 6),  # Path: 2 -> 1 -> 3
        # Tree with negatives - max path avoids negative branch
        ([-10, 9, 20, None, None, 15, 7], 42),  # 15+20+7
        # All negative values, max single node chosen
        ([-3, -2, -1], -1),
        # Complex tree with multiple max paths
        ([5, 4, 8, 11, None, 13, 4, 7, 2, None, None, None, 1], 48),
        # Maximum path includes 7+11+4+5+8+13
        # Tree with zeros
        ([0, -1, 1], 1),
        # Larger tree
        ([10, 2, 10, 20, 1, -25, None, None, None, None, None, 3, 4], 42),
        # Path: 20+2+10+10
    ],
)
def test_max_path_sum(tree_list, expected):
    root = build_tree(tree_list)
    assert Solution().maxPathSum(root) == expected
