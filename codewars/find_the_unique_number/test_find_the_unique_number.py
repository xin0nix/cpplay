import pytest

from typing import Dict, List
from collections import defaultdict


def find_uniq(arr: List[int]):
    # your code here
    seen: Dict[int, int] = defaultdict(int)
    for idx, num in enumerate(arr):
        seen[num] += 1
        # early exit
        if idx > 2 and len(seen) == 2:
            break
    for num in seen:
        if seen[num] == 1:
            return num
    raise Exception("Not expected")

@pytest.mark.parametrize(
    "arr, expected",
    [  # Provided examples
        ([1, 1, 1, 2, 1, 1], 2),
        ([0, 0, 0.55, 0, 0], 0.55),
        # Unique at the beginning
        ([5, 9, 9, 9, 9], 5),
        # Unique at the end
        ([4, 4, 4, 2], 2),
        # Unique in the middle
        ([3, 3, 7, 3, 3], 7),
        # All negative numbers
        ([0, 0, -1, 0, 0], -1),
        # Unique is zero; others are negative
        ([-2, -2, 0, -2, -2], 0),
        # Unique in a large array (unique at start)
        ([10] + [20] * 10000, 10),
        # Unique in a large array (unique at end)
        ([7] * 10000 + [13], 13),
        # Unique in a huge array (unique in the middle)
        ([42] * 5000 + [19] + [42] * 5000, 19),
        # All floats
        ([3.14, 2.71, 3.14, 3.14, 3.14], 2.71),
        ([0.001, 0.001, 0.001, 0.002, 0.001], 0.002),
        # Large floats
        ([1e12, 1e12, 4e11, 1e12], 4e11),
        # Small floats
        ([1e-15, 1e-15, 2e-15, 1e-15], 2e-15),
        # Only three elements
        ([9, 8, 8], 9),
        ([7, 7, 69], 69),
        ([100, 300, 100], 300),
        # Unique is a negative float
        ([6.6, 6.6, -3.3, 6.6], -3.3),
        # Unique with big integer among small ints
        ([1, 1, 10**18, 1, 1], 10**18),
        # String representation of numbers (should not happen, but for resilience)
        # ([2, 2, "3", 2, 2], "3"),
        # Random scattered arrangement
        ([8, 8, 8, 2, 8, 8, 8], 2),
        ([99, 99, 88, 99, 99, 99, 99], 88),
        # Negative unique, positive majority
        ([5, 5, 5, -7, 5], -7),
        # Fractional unique, integer major
        ([1, 1, 1.5, 1, 1, 1], 1.5),
    ],
)
def test_find_uniq(arr: List[int], expected: List[int]):
    assert find_uniq(arr) == expected
