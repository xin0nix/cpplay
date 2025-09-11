import pytest


class Solution:
    def merge(self, intervals: list[list[int]]) -> list[list[int]]:
        if len(intervals) <= 1:
            return intervals
        intervals.sort(key=lambda x: x[0])
        output: list[list[int]] = [intervals[0]]
        for start, end in intervals[1:]:
            _, prev_end = output[-1]
            if prev_end >= start:
                output[-1][1] = max(end, prev_end)
            else:
                output.append([start, end])
        return output


@pytest.mark.parametrize(
    "intervals, expected",
    [
        # Basic overlapping intervals
        ([[1, 3], [2, 6], [8, 10], [15, 18]], [[1, 6], [8, 10], [15, 18]]),
        # Intervals that just touch (end == start)
        ([[1, 4], [4, 5]], [[1, 5]]),
        # Completely nested intervals
        ([[1, 10], [2, 5], [6, 9]], [[1, 10]]),
        # No overlapping intervals
        ([[1, 2], [3, 4], [5, 6]], [[1, 2], [3, 4], [5, 6]]),
        # Single interval input
        ([[1, 4]], [[1, 4]]),
        # Empty input
        ([], []),
        # Intervals out of order
        ([[5, 6], [1, 3], [2, 4]], [[1, 4], [5, 6]]),
        # Large interval containing smaller intervals
        ([[1, 100], [2, 3], [4, 50]], [[1, 100]]),
        # Overlapping intervals with exact same ranges
        ([[1, 3], [1, 3], [1, 3]], [[1, 3]]),
    ],
)
def test_merge_intervals(intervals, expected):
    assert Solution().merge(intervals) == expected
