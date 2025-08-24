import pytest

from typing import Set, List


def sum_pairs(nums: List[int], target: int):
    seen: Set[int] = set()
    for n in nums:
        diff = target - n
        if diff in seen:
            return [diff, n]
        seen.add(n)
    return None


@pytest.mark.parametrize(
    "ints, s, expected",
    [
        # Basic cases
        ([11, 3, 7, 5], 10, [3, 7]),
        ([4, 3, 2, 3, 4], 6, [4, 2]),
        ([0, 0, -2, 3], 2, None),
        ([10, 5, 2, 3, 7, 5], 10, [3, 7]),
        # Negative numbers
        ([-3, 1, 4, 6], 3, [-3, 6]),
        ([-3, -2, -1, 0], -3, [-2, -1]),
        # Duplicates
        ([1, 1, 2, 3, 1, 2], 3, [1, 2]),
        ([1, 2, 3, 2, 1], 4, [1, 3]),
        # No pairs
        ([1, 2, 3], 7, None),
        ([], 0, None),
        ([10], 10, None),
        # Smallest second index
        ([-5, 1, 3, 3, 2, 4], 6, [3, 3]),
    ],
)
def test_sum_pairs(ints, s, expected):
    assert sum_pairs(ints, s) == expected
