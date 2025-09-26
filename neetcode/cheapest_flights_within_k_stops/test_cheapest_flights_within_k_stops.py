import pytest

from typing import List

# Pretend Solution class is imported with method findCheapestPrice
# from solution import Solution


class Solution:
    def findCheapestPrice(
        self, n: int, flights: List[List[int]], src: int, dst: int, k: int
    ) -> int:
        prices = [float("inf") for _ in range(n)]
        prices[src] = 0

        for _ in range(k + 1):
            tmp_prices = prices.copy()
            for [f, t, p] in flights:  # f=from, t=to, p=price
                if prices[f] == float("inf"):
                    continue
                new_price = prices[f] + p
                if new_price < tmp_prices[t]:
                    tmp_prices[t] = new_price
            prices = tmp_prices

        return -1 if prices[dst] == float("inf") else int(prices[dst])


@pytest.mark.parametrize(
    "n, flights, src, dst, k, expected",
    [
        (
            3,
            [[0, 1, 100], [1, 2, 100], [0, 2, 500]],
            0,
            2,
            1,
            200,
        ),  # path with 1 stop cheaper than direct
        (
            3,
            [[0, 1, 100], [1, 2, 100], [0, 2, 500]],
            0,
            2,
            0,
            500,
        ),  # no stops allowed, must take direct flight
        (
            4,
            [[0, 1, 100], [1, 2, 100], [2, 3, 100], [0, 3, 500]],
            0,
            3,
            1,
            500,
        ),  # must take 2 stops but k=1 so direct only
        (
            5,
            [[0, 1, 5], [1, 2, 5], [0, 3, 2], [3, 1, 2], [1, 4, 1], [4, 2, 1]],
            0,
            2,
            2,
            7,
        ),  # multi stop cheaper path
        (
            5,
            [[0, 1, 5], [1, 2, 5], [0, 3, 2], [3, 1, 2], [1, 4, 1], [4, 2, 1]],
            0,
            2,
            1,
            10,
        ),  # restricted stops, less cheap route
        (
            3,
            [[0, 1, 100], [1, 2, 100]],
            0,
            2,
            0,
            -1,
        ),  # no direct flight and no stops allowed
        (1, [], 0, 0, 0, 0),  # trivial case, same src and dst
    ],
)
def test_findCheapestPrice(n, flights, src, dst, k, expected):
    sol = Solution()
    assert sol.findCheapestPrice(n, flights, src, dst, k) == expected
