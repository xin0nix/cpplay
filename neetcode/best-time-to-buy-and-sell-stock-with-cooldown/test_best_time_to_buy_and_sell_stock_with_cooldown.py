from typing import List
from functools import lru_cache
import pytest


class Solution:
    @lru_cache(maxsize=2 * 5000)
    def dfs(self, index: int, holding: bool) -> int:
        if index >= len(self.prices):
            return 0
        current_price = self.prices[index]
        if holding:
            # Option 1: sell now, cooldown next day
            sell = current_price + self.dfs(index + 2, False)  # cooldown after sale
            # Option 2: hold
            skip = self.dfs(index + 1, True)
            return max(sell, skip)
        else:
            # Option 1: buy now
            buy = -current_price + self.dfs(index + 1, True)
            # Option 2: skip
            skip = self.dfs(index + 1, False)
            return max(buy, skip)

    def maxProfit(self, prices: List[int]) -> int:
        self.prices = prices
        return self.dfs(0, False)


@pytest.mark.parametrize(
    "prices, expected",
    [
        ([1, 2, 3, 0, 2], 3),
        ([1], 0),
        ([2, 1], 0),
        ([6, 1, 3, 2, 4, 7], 6),
        ([1, 2, 4], 3),
        ([1, 2, 3, 0, 2, 5], 6),
        (
            [3, 2, 6, 5, 0, 3],
            7,
        ),  # corresponds to multiple buys and sells but cooldown enforced
        ([5, 4, 3, 2, 1], 0),  # decreasing prices, no profit
        (
            [1, 2, 3, 4, 5],
            4,
        ),  # increasing prices, optimal multiple transactions with cooldown
    ],
)
def test_max_profit(
    prices: List[int],
    expected: int,
):
    assert Solution().maxProfit(prices) == expected
