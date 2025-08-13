import pytest
from typing import List


class Solution:
    def plusOne(self, digits: List[int]) -> List[int]:
        result: List[int] = []
        reminder = 1
        for digit in reversed(digits):
            cur = digit + reminder
            reminder = cur // 10
            if cur == 10:
                result.append(0)
            else:
                result.append(cur)
        if reminder != 0:
            result.append(reminder)
        return list(reversed(result))


@pytest.mark.parametrize(
    "input_digits, expected",
    [
        ([1, 2, 3], [1, 2, 4]),
        ([1, 2, 9], [1, 3, 0]),
        ([9, 9, 9], [1, 0, 0, 0]),
        ([5], [6]),
        ([9], [1, 0]),
        ([1, 0, 0], [1, 0, 1]),
        ([0], [1]),
        ([9] * 50, [1] + [0] * 50),
    ],
)
def test_plus_one(input_digits, expected):
    assert Solution().plusOne(input_digits) == expected
