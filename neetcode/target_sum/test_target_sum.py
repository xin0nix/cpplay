import unittest
from typing import List, Dict
from functools import lru_cache
from collections import defaultdict


class Solution:
    def findTargetSumWays(self, nums: List[int], target: int) -> int:
        size = len(nums)
        dp: List[Dict[int, int]] = [defaultdict(int) for _ in range(size + 1)]
        dp[0][0] = 1
        for idx in range(1, size + 1):
            prev_layer = dp[idx - 1]
            cur_layer = dp[idx]
            cur_num = nums[idx - 1]
            for val in prev_layer:
                count = prev_layer[val]
                cur_layer[val + cur_num] += count
                cur_layer[val - cur_num] += count
        return dp[size][target]
        # @lru_cache
        # def dfs(idx: int, remaining: int) -> int:
        #     if idx == size:
        #         return 1 if remaining == 0 else 0
        #     num = nums[idx]
        #     next_idx = idx + 1
        #     return dfs(next_idx, remaining + num) + dfs(next_idx, remaining - num)

        # return dfs(0, target)


class TestTargetSum(unittest.TestCase):
    def setUp(self):
        self.solution = Solution()

    def test_empty_nums(self):
        self.assertEqual(self.solution.findTargetSumWays([], 0), 1)
        self.assertEqual(self.solution.findTargetSumWays([], 5), 0)

    def test_single_element(self):
        self.assertEqual(self.solution.findTargetSumWays([1], 1), 1)
        self.assertEqual(self.solution.findTargetSumWays([1], -1), 1)
        self.assertEqual(self.solution.findTargetSumWays([1], 2), 0)
        self.assertEqual(
            self.solution.findTargetSumWays([0], 0), 2
        )  # +0 and -0 both valid

    def test_example_case(self):
        # Example from problem description:
        nums = [1, 1, 1, 1, 1]
        target = 3
        self.assertEqual(self.solution.findTargetSumWays(nums, target), 5)

    def test_no_solution(self):
        self.assertEqual(self.solution.findTargetSumWays([1, 2, 3], 7), 0)


if __name__ == "__main__":
    unittest.main()
