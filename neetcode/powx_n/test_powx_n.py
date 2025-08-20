import pytest
from functools import lru_cache


class Solution:
    def myPow(self, x: float, n: int) -> float:
        if x == 0:
            return 0

        @lru_cache(None)
        def helper(x: float, n: int) -> float:
            if n == 1:
                return x
            if n == 0:
                return 1
            sub_n = n // 2
            sub_x = helper(x, sub_n)
            if n % 2:
                return x * sub_x * sub_x
            return sub_x * sub_x

        temp = helper(x, abs(n))
        if n < 0:
            return 1 / temp
        return temp


@pytest.mark.parametrize(
    "x, n, expected",
    [
        (2.0, 10, 1024.0),  # positive exponent
        (2.0, 0, 1.0),  # zero exponent
        (2.0, -2, 0.25),  # negative exponent
        (0.0, 5, 0.0),  # base zero
        (1.0, 1000, 1.0),  # base one
        (-2.0, 3, -8.0),  # negative base odd exponent
        (-2.0, 4, 16.0),  # negative base even exponent
        (2.0, 31, 2147483648.0),  # large exponent test
        (2.5, 3, 15.625),  # floating point base
        (2.0, -3, 0.125),  # another negative exponent test
    ],
)
def test_myPow(x, n, expected):
    result = Solution().myPow(x, n)
    assert (
        abs(result - expected) < 1e-9
    ), f"Failed for x={x}, n={n}, got {result}, expected {expected}"
