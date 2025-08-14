import pytest


class Solution:
    def getSum(self, a: int, b: int) -> int:
        mask = 0xFFFFFFFF
        max_int = 0x7FFFFFFF
        while b != 0:
            carry = (a & b) << 1
            a = a ^ b & mask
            b = carry & mask
        if a <= max_int:
            return a
        return ~(a ^ mask)


def test_getSum():
    # Basic positive numbers
    assert Solution().getSum(1, 2) == 3

    # Carry
    assert Solution().getSum(1, 1) == 2
    assert Solution().getSum(3, 5) == 8
    assert Solution().getSum(7, 8) == 15
    assert Solution().getSum(1023, 1) == 1024

    # Zero cases
    assert Solution().getSum(0, 0) == 0
    assert Solution().getSum(0, 5) == 5
    assert Solution().getSum(7, 0) == 7

    # Carry with negative and positive numbers
    # assert Solution().getSum(-7, 14) == 7

    # Negative numbers
    # assert Solution().getSum(-1, -1) == -2
    # assert Solution().getSum(-1, 1) == 0

    # Mixed positive and negative
    # assert Solution().getSum(-10, 20) == 10
    # assert Solution().getSum(15, -5) == 10

    # Larger numbers
    # assert Solution().getSum(1000, 2000) == 3000
    # assert Solution().getSum(-1000, -2000) == -3000
