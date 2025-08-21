import pytest

from typing import List, Tuple


def sum_of_intervals(intervals: List[Tuple[int, int]]) -> int:
    # sort intervals
    intervals.sort(key=lambda x: x[0])
    # merge them
    merged = []
    for cur in intervals:
        if len(merged) == 0:
            merged.append(list(cur))
        # Do they interleave?
        if cur[0] <= merged[-1][1]:
            merged[-1][1] = max(merged[-1][1], cur[1])
        else:
            merged.append(list(cur))
    # count
    total = 0
    for cur in merged:
        total += cur[1] - cur[0]
    return total


@pytest.mark.parametrize(
    "intervals, expected",
    [
        ([(1, 2), (6, 10), (11, 15)], 9),
        ([(1, 4), (7, 10), (3, 5)], 7),
        ([(1, 5), (10, 20), (1, 6), (16, 19), (5, 11)], 19),
        ([(0, 20), (-100000000, 10), (30, 40)], 100000030),
        ([(-1, 1), (1, 2), (2, 3)], 4),
        ([(0, 0)], 0),
        ([(1, 5), (10, 20), (1, 6), (16, 19), (5, 11)], 19),
        ([(0, 20), (-100000000, 10), (30, 40)], 100000030),
    ],
)
def test_sum_of_intervals(intervals, expected):
    assert sum_of_intervals(intervals) == expected
