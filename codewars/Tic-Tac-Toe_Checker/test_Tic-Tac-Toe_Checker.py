import pytest

from typing import List


def is_solved(board: List[List[int]]):
    def allAre(xs: List[int], val: int) -> bool:
        for x in xs:
            if x != val:
                return False
        return True

    # Check straight lines
    for i in range(3):
        row = board[i]
        col = [board[0][i], board[1][i], board[2][i]]
        if allAre(row, 1) or allAre(col, 1):
            return 1
        if allAre(row, 2) or allAre(col, 2):
            return 2
    # Check diagonals
    diag_1 = [board[0][0], board[1][1], board[2][2]]
    diag_2 = [board[0][2], board[1][1], board[2][0]]
    if allAre(diag_1, 1) or allAre(diag_2, 1):
        return 1
    if allAre(diag_1, 2) or allAre(diag_2, 2):
        return 2
    for _, row in enumerate(board):
        if 0 in row:
            return -1
    return 0


@pytest.mark.parametrize(
    "board, expected",
    [
        # unfinished board: empty spots and no winner
        ([[0, 0, 1], [0, 1, 2], [2, 1, 0]], -1),
        # X wins (horizontal)
        ([[1, 1, 1], [0, 2, 0], [2, 0, 2]], 1),
        # X wins (vertical)
        ([[1, 2, 0], [1, 2, 0], [1, 0, 2]], 1),
        # X wins (diagonal)
        ([[1, 2, 0], [0, 1, 2], [0, 0, 1]], 1),
        # O wins (horizontal)
        ([[2, 2, 2], [1, 1, 0], [0, 1, 0]], 2),
        # O wins (vertical)
        ([[1, 2, 0], [1, 2, 0], [0, 2, 1]], 2),
        # O wins (diagonal)
        ([[1, 1, 2], [1, 2, 0], [2, 0, 0]], 2),
        # cat's game (draw)
        ([[1, 2, 1], [1, 2, 2], [2, 1, 1]], 0),
    ],
)
def test_is_solved(board, expected):
    assert is_solved(board) == expected
