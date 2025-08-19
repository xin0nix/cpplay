import pytest
from typing import List


class Solution:
    def spiralOrder(self, matrix: List[List[int]]) -> List[int]:
        result: List[int] = []

        def add(i, j):
            val = matrix[i][j]
            print(f"Adding ({i},{j}) = {val}")
            result.append(val)

        # Implement the solution or import if implemented somewhere else
        def traverse(left, right, top, bottom):
            if left >= right or top >= bottom:
                return
            for col in range(left, right):
                add(top, col)
            top += 1

            for row in range(top, bottom):
                add(row, right - 1)
            right -= 1

            if left >= right or top >= bottom:
                return

            for col in range(right - 1, left - 1, -1):
                add(bottom - 1, col)
            bottom -= 1

            for row in range(bottom - 1, top - 1, -1):
                add(row, left)
            left += 1

            traverse(left, right, top, bottom)

        traverse(0, len(matrix[0]), 0, len(matrix))

        return result


@pytest.mark.parametrize(
    "matrix, expected",
    [
        # Basic test cases
        ([[1, 2, 3], [4, 5, 6], [7, 8, 9]], [1, 2, 3, 6, 9, 8, 7, 4, 5]),
        (
            [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]],
            [1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7],
        ),
        ([[1]], [1]),
        ([[1, 2], [3, 4], [5, 6]], [1, 2, 4, 6, 5, 3]),
        # Single row matrix
        ([[1, 2, 3, 4, 5]], [1, 2, 3, 4, 5]),
        # Rectangular wide matrix
        ([[1, 2, 3, 4, 5], [6, 7, 8, 9, 10]], [1, 2, 3, 4, 5, 10, 9, 8, 7, 6]),
        # Rectangular tall matrix
        ([[1, 2], [3, 4], [5, 6], [7, 8]], [1, 2, 4, 6, 8, 7, 5, 3]),
        # Larger square matrix 4x4
        (
            [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]],
            [1, 2, 3, 4, 8, 12, 16, 15, 14, 13, 9, 5, 6, 7, 11, 10],
        ),
        # Matrix with identical elements
        ([[5, 5, 5], [5, 5, 5], [5, 5, 5]], [5, 5, 5, 5, 5, 5, 5, 5, 5]),
        # Matrix with negative values
        (
            [[-1, -2, -3], [-4, -5, -6], [-7, -8, -9]],
            [-1, -2, -3, -6, -9, -8, -7, -4, -5],
        ),
    ],
)
def test_spiralOrder(matrix, expected):
    assert Solution().spiralOrder(matrix) == expected
