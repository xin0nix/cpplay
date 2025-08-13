import pytest
from heapq import heappush, heappop
from typing import List, Tuple


class Solution:
    def maxSlidingWindow(self, nums: List[int], win_len: int) -> List[int]:
        max_heap: List[Tuple[int, int]] = []
        result: List[int] = []
        # Initialization (no need to remove items from the heap)
        for end_pos, num in enumerate(nums[:win_len]):
            heappush(max_heap, (-num, end_pos))
        result.append(-max_heap[0][0])

        for end_pos in range(win_len, len(nums)):
            num = nums[end_pos]
            heappush(max_heap, (-num, end_pos))
            win_start_pos = end_pos - win_len + 1
            # Remove elements which went out of the window (if any)
            while max_heap[0][1] < win_start_pos:
                heappop(max_heap)
            # At this point the top element is inside the window (no guarantees about lesser ones)
            result.append(-max_heap[0][0])
        return result


@pytest.mark.parametrize(
    "nums, k, expected",
    [
        # Basic test with window size 3
        ([1, 3, -1, -3, 5, 3, 6, 7], 3, [3, 3, 5, 5, 6, 7]),
        # Window size = 1, output should be the array itself
        ([4, 2, 12, 11], 1, [4, 2, 12, 11]),
        # Window size equals array length, single max element
        ([9, 10, 9, -7, -4, -8, 2, -6], 8, [10]),
        # Array with negatives, window size 2
        ([-1, -3, -5, -2, -1, -7], 2, [-1, -3, -2, -1, -1]),
        # Decreasing array, window size 4
        ([10, 9, 8, 7, 6, 5], 4, [10, 9, 8]),
        # Increasing array, window size 2
        ([1, 2, 3, 4, 5], 2, [2, 3, 4, 5]),
        # All elements equal, any window size
        ([5, 5, 5, 5, 5], 3, [5, 5, 5]),
        # Edge case: single element array
        ([10], 1, [10]),
    ],
)
def test_max_sliding_window(nums, k, expected):
    assert Solution().maxSlidingWindow(nums, k) == expected
