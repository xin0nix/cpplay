import pytest
import heapq


class MedianFinder:
    def __init__(self):
        # max heap
        self.left_heap = []
        # min heap
        self.right_heap = []

    def pop_left(self):
        return -1 * heapq.heappop(self.left_heap)

    def pop_right(self):
        return heapq.heappop(self.right_heap)

    def add_left(self, x: int):
        heapq.heappush(self.left_heap, -1 * x)

    def add_right(self, x: int):
        heapq.heappush(self.right_heap, x)

    def get_max_left(self):
        return -1 * self.left_heap[0]

    def get_min_right(self):
        return self.right_heap[0]

    def addNum(self, num: int) -> None:
        if self.right_heap and num > self.get_min_right():
            self.add_right(num)
        else:
            self.add_left(num)

        if len(self.left_heap) > len(self.right_heap) + 1:
            x = self.pop_left()
            self.add_right(x)
        if len(self.right_heap) > len(self.left_heap) + 1:
            x = self.pop_right()
            self.add_left(x)

    def findMedian(self) -> float:
        if len(self.left_heap) == len(self.right_heap):
            l = self.get_max_left()
            r = self.get_min_right()
            return (l + r) / 2
        if len(self.left_heap) > len(self.right_heap):
            return self.get_max_left()
        return self.get_min_right()


# Your MedianFinder object will be instantiated and called as such:
# obj = MedianFinder()
# obj.addNum(num)
# param_2 = obj.findMedian()


@pytest.mark.parametrize(
    "left, right, expected",
    [
        ([1, 2, 3], [4, 5, 6], (3.5)),
        ([-9, -8], [-6, -4], -7),
        ([], [1], 1),
        ([1], [], 1),
        ([-2, -1, 0], [2, 3], 0),
    ],
)
def test_find_median(left, right, expected):
    finder = MedianFinder()
    for l in left:
        finder.add_left(l)
    for r in right:
        finder.add_right(r)
    median = finder.findMedian()
    assert median == expected


@pytest.mark.parametrize(
    "nums, expected",
    [
        ([1, 2, 3, 4, 5, 6], (3.5)),
        ([-9, -8, -6, -4], -7),
        ([1], 1),
        ([-2, -1, 0, 2, 3], 0),
    ],
)
def test_find_median2(nums, expected):
    finder = MedianFinder()
    for x in nums:
        finder.addNum(x)
    median = finder.findMedian()
    assert median == expected
