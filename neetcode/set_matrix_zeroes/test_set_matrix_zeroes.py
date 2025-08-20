import pytest

from typing import List


class Solution:
    def setZeroes(self, matrix: List[List[int]]) -> None:
        """
        Do not return anything, modify matrix in-place instead.
        """
        rows = [False] * len(matrix)
        cols = [False] * len(matrix[0])
        # 1. Заполним массивы rows и cols
        for i, row in enumerate(matrix):
            for j, cell in enumerate(row):
                if cell == 0:
                    rows[i] = True
                    cols[j] = True
        # 2. Согласно собранным rows обновим matrix
        for i, mark in enumerate(rows):
            if not mark:
                continue
            for j in range(len(cols)):
                matrix[i][j] = 0
        # 3. Согласно собранным cols обновим matrix
        for j, mark in enumerate(cols):
            if not mark:
                continue
            for i in range(len(rows)):
                matrix[i][j] = 0


@pytest.mark.parametrize(
    "input_matrix, expected",
    [
        # Previous cases omitted for brevity
        # Larger matrix with multiple zero rows and columns
        (
            [[1, 0, 3, 4], [0, 6, 7, 8], [9, 10, 0, 12], [13, 14, 15, 16]],
            [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 16]],
        ),
        # Matrix with all zeroes
        ([[0, 0], [0, 0]], [[0, 0], [0, 0]]),
        # Matrix with zero only in last element
        ([[1, 2, 3], [4, 5, 6], [7, 8, 0]], [[1, 2, 0], [4, 5, 0], [0, 0, 0]]),
        # Matrix with zeroes in every row but different columns
        ([[0, 1, 2], [3, 0, 4], [5, 6, 0]], [[0, 0, 0], [0, 0, 0], [0, 0, 0]]),
        # Matrix with a single row and zero
        ([[1, 0, 3]], [[0, 0, 0]]),
    ],
)
def test_setZeroes_extended(input_matrix: List[List[int]], expected: List[List[int]]):
    Solution().setZeroes(input_matrix)
    assert input_matrix == expected
