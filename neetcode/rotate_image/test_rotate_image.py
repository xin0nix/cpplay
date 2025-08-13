import pytest

from typing import List


class Solution:
    def rotate(self, matrix: list[list[int]]) -> None:
        n = len(matrix)

        def top(layer, k):
            return (layer, layer + k)

        def right(layer, k):
            return (layer + k, n - layer - 1)

        def bottom(layer, k):
            return (n - layer - 1, n - layer - k - 1)

        def left(layer, k):
            return (n - layer - k - 1, layer)

        for layer in range(n // 2):
            layer_size = n - 2 * layer
            for k in range(layer_size - 1):
                top_i, top_j = top(layer, k)
                right_i, right_j = right(layer, k)
                bottom_i, bottom_j = bottom(layer, k)
                left_i, left_j = left(layer, k)

                cur_top = matrix[top_i][top_j]
                cur_right = matrix[right_i][right_j]
                cur_bottom = matrix[bottom_i][bottom_j]
                cur_left = matrix[left_i][left_j]

                matrix[right_i][right_j] = cur_top
                matrix[bottom_i][bottom_j] = cur_right
                matrix[left_i][left_j] = cur_bottom
                matrix[top_i][top_j] = cur_left


@pytest.mark.parametrize(
    "matrix, expected",
    [
        # 1x1 matrix (edge case)
        ([[1]], [[1]]),
        # 2x2 matrix
        ([[1, 2], [3, 4]], [[3, 1], [4, 2]]),
        # 3x3 matrix
        ([[1, 2, 3], [4, 5, 6], [7, 8, 9]], [[7, 4, 1], [8, 5, 2], [9, 6, 3]]),
        # 4x4 matrix
        (
            [[5, 1, 9, 11], [2, 4, 8, 10], [13, 3, 6, 7], [15, 14, 12, 16]],
            [[15, 13, 2, 5], [14, 3, 4, 1], [12, 6, 8, 9], [16, 7, 10, 11]],
        ),
    ],
)
def test_rotate(matrix, expected):
    sol = Solution()
    sol.rotate(matrix)
    assert matrix == expected
