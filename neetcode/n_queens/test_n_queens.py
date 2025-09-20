import pytest

from typing import List, Dict, Set


class Solution:
    def prepare(self, n) -> List[List[str]]:
        return [["."] * n for _ in range(n)]

    def finalize(self, board: List[List[str]]) -> List[str]:
        return ["".join(r) for r in board]

    def solveNQueens(self, n: int) -> List[List[str]]:
        board = self.prepare(n)
        res: List[List[str]] = []
        cols: Set[int] = set()
        pos_diags: Set[int] = set()  # (r+c) == const
        neg_diags: Set[int] = set()  # (r-c) == const

        def backtrack(r: int):
            if r == n:
                res.append(self.finalize(board))
                return

            for c in range(n):
                pos = r + c
                neg = r - c
                if (c in cols) or (pos in pos_diags) or (neg in neg_diags):
                    continue

                board[r][c] = "Q"
                cols.add(c)
                pos_diags.add(pos)
                neg_diags.add(neg)
                backtrack(r + 1)
                neg_diags.remove(neg)
                pos_diags.remove(pos)
                cols.remove(c)
                board[r][c] = "."

        backtrack(0)
        return res


@pytest.mark.parametrize(
    "n, expected_solutions",
    [
        (1, [["Q"]]),
        (2, []),  # No solution for 2 queens
        (3, []),  # No solution for 3 queens
        (4, [[".Q..", "...Q", "Q...", "..Q."], ["..Q.", "Q...", "...Q", ".Q.."]]),
    ],
)
def test_n_queens_specific_solutions(n, expected_solutions):
    assert sorted(Solution().solveNQueens(n)) == sorted(expected_solutions)


def test_prepare_boards():
    s = Solution()

    # Test 1: n=1 board
    n = 1
    expected = ["."]
    assert s.finalize(s.prepare(n)) == expected

    # Test 2: n=3 board
    n = 3
    expected = ["...", "...", "..."]
    assert s.finalize(s.prepare(n)) == expected

    # Test 3: n=0 edge case (empty board)
    n = 0
    expected = []
    assert s.finalize(s.prepare(n)) == expected

    # Test 4: n=5 board
    n = 5
    expected = [".....", ".....", ".....", ".....", "....."]
    assert s.finalize(s.prepare(n)) == expected
