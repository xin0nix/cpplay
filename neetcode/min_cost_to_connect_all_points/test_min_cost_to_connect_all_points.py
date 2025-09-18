import unittest

import heapq
from typing import List, Tuple, Dict, Set
from collections import defaultdict


class Solution:
    def calculateAdjLists(
        self, points: List[List[int]]
    ) -> Dict[int, List[Tuple[int, int]]]:
        # point -> [cost, another point]
        adj: Dict[int, List[Tuple[int, int]]] = defaultdict(list)
        for i, [x0, y0] in enumerate(points):
            for j, [x1, y1] in enumerate(points[i + 1 :]):
                j += i + 1
                cost = abs(x0 - x1) + abs(y0 - y1)
                adj[i].append((cost, j))
                adj[j].append((cost, i))
        return adj

    def minCostConnectPoints(self, points: List[List[int]]) -> int:
        adj = self.calculateAdjLists(points)
        seen: Set[int] = set()
        # min heap of [(cost, point)]
        heap: List[Tuple[int, int]] = [(0, 0)]
        res = 0
        while len(seen) < len(points):
            cost, i = heapq.heappop(heap)
            if i in seen:
                continue
            res += cost
            seen.add(i)
            for nei_cost, nei_i in adj[i]:
                if nei_i not in seen:
                    heapq.heappush(heap, (nei_cost, nei_i))
        return res


class TestCalculateAdjLists(unittest.TestCase):
    def setUp(self):
        self.helper = Solution()

    def test_empty_points(self):
        points = []
        expected = {}
        result = self.helper.calculateAdjLists(points)
        self.assertEqual(result, expected)

    def test_two_points(self):
        points = [[0, 0], [2, 3]]
        # cost = |0-2|+|0-3|=5
        expected = defaultdict(list)
        expected[0] = [(5, 1)]
        expected[1] = [(5, 0)]
        result = self.helper.calculateAdjLists(points)
        self.assertEqual(result, expected)

    def test_three_points(self):
        points = [[0, 0], [1, 1], [2, 2]]
        # adjacencies:
        # 0: (2 to 1), (4 to 2)
        # 1: (2 to 0), (2 to 2)
        # 2: (4 to 0), (2 to 1)
        expected = defaultdict(list)
        expected[0] = [(2, 1), (4, 2)]
        expected[1] = [(2, 0), (2, 2)]
        expected[2] = [(4, 0), (2, 1)]
        result = self.helper.calculateAdjLists(points)
        self.assertEqual(result, expected)

    def test_non_empty_values(self):
        points = [[1, 5], [3, 7]]
        # cost = |1-3|+|5-7|=4
        expected = defaultdict(list)
        expected[0] = [(4, 1)]
        expected[1] = [(4, 0)]
        result = self.helper.calculateAdjLists(points)
        self.assertEqual(result, expected)


class TestMinCostToConnectAllPoints(unittest.TestCase):
    def setUp(self):
        self.sol = Solution()

    def test_single_point(self):
        points = [[0, 0]]
        expected = 0
        self.assertEqual(self.sol.minCostConnectPoints(points), expected)

    def test_two_points(self):
        points = [[0, 0], [2, 3]]
        expected = 5  # |0-2| + |0-3| = 5
        self.assertEqual(self.sol.minCostConnectPoints(points), expected)

    def test_three_points_line(self):
        points = [[0, 0], [1, 1], [2, 2]]
        expected = 4  # Connect in sequence: 1 + 1 + 2 = 4 total minimum cost MST
        self.assertEqual(self.sol.minCostConnectPoints(points), expected)

    def test_four_points_square(self):
        points = [[0, 0], [0, 1], [1, 0], [1, 1]]
        expected = 3  # MST edges cost: 1 + 1 + 1 = 3
        self.assertEqual(self.sol.minCostConnectPoints(points), expected)

    def test_example_case(self):
        points = [[0, 0], [2, 2], [3, 10], [5, 2], [7, 0]]
        expected = 20
        self.assertEqual(self.sol.minCostConnectPoints(points), expected)

    def test_negative_coordinates(self):
        points = [[-1, -1], [-2, -2], [1, 1]]
        # MST edges cost includes connecting (-2,-2) to (-1,-1) and (-1,-1) to (1,1)
        expected = 6
        self.assertEqual(self.sol.minCostConnectPoints(points), expected)

    def test_large_coordinates(self):
        points = [[1000000, 1000000], [-1000000, -1000000]]
        expected = 4000000  # |1000000 + 1000000| + |1000000 + 1000000| = 4,000,000
        self.assertEqual(self.sol.minCostConnectPoints(points), expected)


if __name__ == "__main__":
    unittest.main()
