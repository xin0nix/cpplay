import pytest

from typing import Optional


# Sample TreeNode definition for tests (if needed)
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val: int = val
        self.left = left
        self.right = right


class Codec:

    # Encodes a tree to a single string.
    def serialize(self, root: Optional[TreeNode]) -> str:
        res: list[str] = []

        def dfs(node: Optional[TreeNode]):
            if not node:
                res.append("N")
                return
            res.append(f"{node.val}")
            dfs(node.left)
            dfs(node.right)

        dfs(root)

        return ",".join(res)

    # Decodes your encoded data to tree.
    def deserialize(self, data: str) -> Optional[TreeNode]:
        if not data:
            return None

        nums = list(data.split(","))

        idx = -1

        def dfs() -> Optional[TreeNode]:
            nonlocal idx
            idx += 1
            print(f"Visiting {idx}")
            if nums[idx] == "N":
                return None
            node = TreeNode(int(nums[idx]))
            node.left = dfs()
            node.right = dfs()
            return node

        root = dfs()
        print(self.serialize(root))
        return root


def tree_equal(t1, t2):
    # Helper function to compare two binary trees for equality
    if not t1 and not t2:
        return True
    if not t1 or not t2:
        return False
    return (
        (t1.val == t2.val)
        and tree_equal(t1.left, t2.left)
        and tree_equal(t1.right, t2.right)
    )


@pytest.fixture
def codec():
    return Codec()


def test_empty_tree(codec):
    root = None
    serialized = codec.serialize(root)
    deserialized = codec.deserialize(serialized)
    assert deserialized is None


def test_single_node(codec):
    root = TreeNode(1)
    serialized = codec.serialize(root)
    deserialized = codec.deserialize(serialized)
    assert tree_equal(root, deserialized)


def test_complete_tree(codec):
    # Construct tree:      1
    #                    /   \
    #                   2     3
    #                        / \
    #                       4   5
    root = TreeNode(1, TreeNode(2), TreeNode(3, TreeNode(4), TreeNode(5)))
    serialized = codec.serialize(root)
    deserialized = codec.deserialize(serialized)
    assert tree_equal(root, deserialized)


def test_left_skewed_tree(codec):
    # Tree: 1 -> 2 -> 3 -> 4 (all left children)
    root = TreeNode(1, TreeNode(2, TreeNode(3, TreeNode(4))))
    serialized = codec.serialize(root)
    deserialized = codec.deserialize(serialized)
    assert tree_equal(root, deserialized)


def test_right_skewed_tree(codec):
    # Tree: 1 -> 2 -> 3 -> 4 (all right children)
    root = TreeNode(1, None, TreeNode(2, None, TreeNode(3, None, TreeNode(4))))
    serialized = codec.serialize(root)
    deserialized = codec.deserialize(serialized)
    assert tree_equal(root, deserialized)


def test_tree_with_null_nodes(codec):
    # Tree:    1
    #         / \
    #        2   None
    #         \
    #          3
    root = TreeNode(1, TreeNode(2, None, TreeNode(3)), None)
    serialized = codec.serialize(root)
    deserialized = codec.deserialize(serialized)
    assert tree_equal(root, deserialized)
