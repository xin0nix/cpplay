import pytest

from typing import List


class Solution:
    def merge(self, a: List[int], intervals: List[List[int]]) -> List[int]:
        """
        merges interval a with a range of intervals
        this function assumes that a overlaps with each interval
        """
        if len(intervals) == 0:
            return a
        b0 = intervals[0]
        bn = intervals[-1]
        return [min(a[0], b0[0]), max(a[1], bn[1])]

    def compare(self, a: List[int], b: List[int]) -> int:
        """
        -1 means a is before b (on the left)
        0 means a is overlapping with b
        1 means a is after b (on the right)
        """
        if a[1] < b[0]:
            return -1
        if a[0] > b[1]:
            return 1
        return 0

    def partition(
        self, intervals: List[List[int]], newInterval: List[int]
    ) -> List[List[int]]:
        """
        partition intervals into 3 groups depending their relative positions to newInterval
        a) First group consists of all the intervals on the left of newInterval (may be [])
        b) Second group is of all the intervals that overlap with newInterval (may be [])
        c) Third group is of all the intervals on the right from the newInterval (may be [])
        """
        first: List[int] = []
        second: List[int] = []
        third: List[int] = []
        for key, val in enumerate(intervals):
            if self.compare(val, newInterval) == -1:
                first.append(key)
            elif self.compare(val, newInterval) == 0:
                second.append(key)
            else:
                third.append(key)
        return [first, second, third]

    def insert(
        self, intervals: List[List[int]], newInterval: List[int]
    ) -> List[List[int]]:
        [lesser, overlap, greater] = self.partition(intervals, newInterval)
        res: List[List[int]] = []
        for i in lesser:
            res.append(intervals[i])
        overlapping: List[List[int]] = []
        for i in overlap:
            overlapping.append(intervals[i])
        merged = self.merge(newInterval, overlapping)
        res.append(merged)
        for i in greater:
            res.append(intervals[i])
        return res


@pytest.mark.parametrize(
    "intervals,newInterval,expected",
    [
        (
            [[1, 3], [6, 9]],
            [2, 5],
            [[1, 5], [6, 9]],
        ),  # Overlapping with the first interval
        (
            [[1, 2], [3, 5], [6, 7], [8, 10], [12, 16]],
            [4, 8],
            [[1, 2], [3, 10], [12, 16]],
        ),  # Overlapping with multiple intervals
        ([], [5, 7], [[5, 7]]),  # Empty intervals, just insert newInterval
        (
            [[1, 5]],
            [2, 3],
            [[1, 5]],
        ),  # newInterval completely inside an existing interval
        ([[1, 5]], [6, 8], [[1, 5], [6, 8]]),  # newInterval after existing intervals
        ([[1, 5]], [0, 0], [[0, 0], [1, 5]]),  # newInterval before existing intervals
        (
            [[1, 3], [6, 9]],
            [10, 11],
            [[1, 3], [6, 9], [10, 11]],
        ),  # newInterval after all intervals
        (
            [[1, 3], [6, 9]],
            [0, 0],
            [[0, 0], [1, 3], [6, 9]],
        ),  # newInterval before all intervals
        (
            [[1, 3], [6, 9], [12, 15]],
            [8, 13],
            [[1, 3], [6, 15]],
        ),  # Overlapping with middle and last intervals
    ],
)
def test_insert_interval(intervals, newInterval, expected):
    assert Solution().insert(intervals, newInterval) == expected


@pytest.mark.parametrize(
    "a, intervals, expected",
    [
        (
            [5, 8],
            [[3, 6], [7, 9], [10, 12]],
            [3, 12],
        ),  # intervals do not overlap each other, but overlap a
        (
            [4, 10],
            [[2, 5], [6, 7], [8, 12]],
            [2, 12],
        ),  # intervals separate, a overlaps all
        (
            [10, 15],
            [[9, 11], [12, 14], [14, 16]],
            [9, 16],
        ),  # intervals consecutive but non-overlapping, all overlap a
    ],
)
def test_merge(a, intervals, expected):
    helper = Solution()
    assert helper.merge(a, intervals) == expected


@pytest.mark.parametrize(
    "a,b,expected",
    [
        ([1, 3], [4, 6], -1),  # a is before b
        ([4, 6], [1, 3], 1),  # a is after b
        ([1, 5], [4, 8], 0),  # overlapping intervals
        ([4, 8], [1, 5], 0),  # overlapping intervals reversed
        ([1, 3], [3, 5], 0),  # touching intervals considered overlapping
        ([3, 5], [1, 3], 0),  # touching intervals reversed
        ([1, 10], [2, 3], 0),  # b inside a (overlap)
        ([2, 3], [1, 10], 0),  # a inside b (overlap)
    ],
)
def test_compare(a, b, expected):
    helper = Solution()
    assert helper.compare(a, b) == expected


@pytest.mark.parametrize(
    "intervals,newInterval,expected",
    [
        (
            [[1, 3], [5, 7], [8, 12], [13, 15]],
            [6, 10],
            [[0], [1, 2], [3]],
        ),  # overlap middle intervals
        ([[1, 4], [6, 7], [9, 11]], [5, 8], [[0], [1], [2]]),  # overlap one interval
        (
            [[1, 2], [3, 5], [6, 7]],
            [8, 10],
            [[0, 1, 2], [], []],
        ),  # all intervals left of newInterval
        (
            [[11, 13], [14, 16], [17, 19]],
            [5, 10],
            [[], [], [0, 1, 2]],
        ),  # all intervals right of newInterval
        ([], [2, 3], [[], [], []]),  # empty intervals list
        (
            [[1, 4], [5, 7], [8, 10]],
            [5, 6],
            [[0], [1], [2]],
        ),  # intervals touching newInterval treated by compare
    ],
)
def test_partition(intervals, newInterval, expected):
    helper = Solution()
    result = helper.partition(intervals, newInterval)
    assert result == expected
