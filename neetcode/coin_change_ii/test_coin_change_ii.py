import unittest
from typing import List
from functools import lru_cache


class Solution:
    def change(self, amount: int, coins: List[int]) -> int:
        if amount == 0:
            return 1

        n = len(coins)
        dp = [[0] * (amount + 1)] * n
        for idx in range(n):
            dp[idx][0] = 1

        def get(coin_idx, amount_idx):
            if coin_idx < 0 or coin_idx >= len(dp):
                return 0
            if amount_idx < 0 or amount_idx >= len(dp[0]):
                return 0
            return dp[coin_idx][amount_idx]

        for coins_row in range(n - 1, -1, -1):
            for cur_amount in range(1, amount + 1):
                coin = coins[coins_row]
                in_row = get(coins_row, cur_amount - coin)
                in_col = get(coins_row + 1, cur_amount)
                dp[coins_row][cur_amount] = in_row + in_col

        return dp[0][amount]
    
        # @lru_cache(None)
        # def dfs(amount: int, start_pos: int) -> int:
        #     if amount == 0:
        #         return 1
        #     total = 0
        #     for pos in range(start_pos, len(coins)):
        #         target = amount - coins[pos]
        #         if target == 0:
        #             total += 1
        #             continue
        #         if target < 0:
        #             continue
        #         total += dfs(target, pos)
        #     return total

        # return dfs(amount, 0)


class TestCoinChangeII(unittest.TestCase):

    def setUp(self):
        self.solution = Solution()

    def test_example_1(self):
        # amount=5, coins=[1,2,5]
        # Combinations:
        # 1) 4 = 2+2
        # 2) 4 = 2+1+1
        # 3) 4 = 1+1+1+1
        # 4) 4 = 3+1
        self.assertEqual(self.solution.change(4, [1, 2, 3]), 4)

    def test_example_2(self):
        # amount=3, coins=[2]
        # Combinations:
        # None, since 2 cannot sum up to 3
        self.assertEqual(self.solution.change(3, [2]), 0)

    def test_example_3(self):
        # amount=10, coins=[10]
        # Combinations:
        # 1) 10 = 10
        self.assertEqual(self.solution.change(10, [10]), 1)

    def test_large_amount(self):
        # amount=100, coins=[1,5,10,25]
        # Too many combinations to list explicitly, but result should be positive
        self.assertTrue(self.solution.change(100, [1, 5, 10, 25]) > 0)

    def test_single_coin_multiple(self):
        # amount=4, coins=[1]
        # Combinations:
        # 1) 1+1+1+1
        self.assertEqual(self.solution.change(4, [1]), 1)

    def test_coins_not_in_order(self):
        # amount=5, coins=[5, 1, 2]
        # Same combinations as test_example_1, order of coins does not matter:
        # 1) 5 = 5
        # 2) 5 = 2+2+1
        # 3) 5 = 2+1+1+1
        # 4) 5 = 1+1+1+1+1
        self.assertEqual(self.solution.change(5, [5, 1, 2]), 4)

    def test_coin_larger_than_amount(self):
        # amount=3, coins=[4, 5, 6]
        # Combinations:
        # None because all coins > 3
        self.assertEqual(self.solution.change(3, [4, 5, 6]), 0)
