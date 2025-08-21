import pytest
from typing import List


def move_zeros(nums: List[int]):
    i = 0
    for j, val in enumerate(nums):
        if val != 0:
            nums[i] = val
            i += 1
    while i < len(nums):
        nums[i] = 0
        i += 1
    return nums


@pytest.mark.parametrize(
    "input_list, expected",
    [
        ([1, 0, 1, 2, 0, 1, 3], [1, 1, 2, 1, 3, 0, 0]),
        ([0, 0, 0, 1, 2, 3], [1, 2, 3, 0, 0, 0]),
        ([1, 2, 3, 0, 0], [1, 2, 3, 0, 0]),
        ([0, 0, 0], [0, 0, 0]),
        ([1, 2, 3], [1, 2, 3]),
        ([], []),
        ([5, 0, 0, 1], [5, 1, 0, 0]),
    ],
)
def test_move_zeros(input_list, expected):
    assert move_zeros(input_list) == expected
