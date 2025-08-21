import pytest

from functools import lru_cache


def find_even_index(arr):
    # First - calculate sums on the right from index
    n = len(arr)
    rsums = [0] * n
    for idx in range(n - 1, -1, -1):
        if idx == n - 1:
            continue
        rsums[idx] = rsums[idx + 1] + arr[idx + 1]
    # Now we can traverse the array from left to right
    lsum = 0
    for idx in range(len(arr)):
        if idx != 0:
            lsum += arr[idx - 1]
        if lsum == rsums[idx]:
            return idx
    return -1


@pytest.mark.parametrize(
    "arr, expected",
    [
        ([1, 2, 3, 4, 3, 2, 1], 3),
        ([1, 100, 50, -51, 1, 1], 1),
        ([20, 10, -80, 10, 10, 15, 35], 0),
        ([1, 2, 3, 4, 5, 6], -1),
        ([10, -80, 10, 10, 15, 35, 20], 6),
        ([1, -1, 0, 0], 2),
        ([-1, -2, -3, -4, -3, -2, -1], 3),
        ([1, 2, 3, 4, 0, 3, 2, 1], 3),
    ],
)
def test_find_even_index(arr, expected):
    assert find_even_index(arr) == expected
