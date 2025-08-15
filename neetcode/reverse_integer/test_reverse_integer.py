import pytest
import math


class Solution:
    def reverse(self, x: int) -> int:
        MIN = -2147483648  # -2^31,
        MAX = 2147483647  #  2^31 - 1

        res = 0
        while x != 0:
            rem = int(math.fmod(x, 10))
            x = int(x / 10)

            if res > MAX // 10 or (res == MAX // 10 and rem > MAX % 10):
                return 0
            if res < MIN // 10 or (res == MIN // 10 and rem < MIN % 10):
                return 0

            res = 10 * res + rem

        return res


import pytest


@pytest.mark.parametrize(
    "input_value, expected",
    [
        # Normal cases
        (123, 321),
        (-123, -321),
        (120, 21),
        (0, 0),
        # Edge cases close to integer limits
        (1534236469, 0),  # overflow after reverse
        (-1534236469, 0),  # overflow after reverse
        (1463847412, 2147483641),
        (-2147483412, -2143847412),
        # Single digit number
        (5, 5),
        # Negative single digit number
        (-8, -8),
        # Number that becomes smaller after reverse
        (10, 1),
    ],
)
def test_reverse_integer(input_value, expected):
    assert Solution().reverse(input_value) == expected
