import pytest

from typing import List, Dict
from dataclasses import dataclass


@dataclass(frozen=True)
class Point:
    x: int
    y: int


class DetectSquares:
    def __init__(self):
        self.points: Dict[Point, int] = {}

    def add(self, point: List[int]) -> None:
        p_x, p_y = point
        p = Point(p_x, p_y)
        if p in self.points:
            self.points[p] += 1
        else:
            self.points[p] = 1

    def count(self, point: List[int]) -> int:
        p_x, p_y = point
        res = 0
        for cur, count in self.points.items():
            x, y = cur.x, cur.y
            dx = abs(p_x - x)
            dy = abs(p_y - y)
            if dx != dy or dx == 0 or dy == 0:
                continue
            if Point(x, p_y) not in self.points or Point(p_x, y) not in self.points:
                continue
            res += count * self.points[Point(x, p_y)] * self.points[Point(p_x, y)]
        return res


@pytest.fixture
def detect_squares():
    return DetectSquares()


def test_add_and_count_basic(detect_squares):
    detect_squares.add([3, 10])
    detect_squares.add([11, 2])
    detect_squares.add([3, 2])
    assert detect_squares.count([11, 10]) == 1  # Forms one square
    assert detect_squares.count([14, 8]) == 0  # No squares can be formed


def test_multiple_occurrences(detect_squares):
    detect_squares.add([1, 1])
    detect_squares.add([1, 1])
    detect_squares.add([1, 2])
    detect_squares.add([2, 1])
    detect_squares.add([2, 2])
    # With duplicate points at (1,1), count should consider multiple squares
    assert detect_squares.count([2, 2]) == 2


def test_no_squares(detect_squares):
    detect_squares.add([0, 0])
    detect_squares.add([1, 0])
    detect_squares.add([0, 1])
    assert detect_squares.count([2, 2]) == 0


def test_single_point_count(detect_squares):
    detect_squares.add([5, 5])
    assert detect_squares.count([5, 5]) == 0


def test_large_coordinates(detect_squares):
    detect_squares.add([1000000, 1000000])
    detect_squares.add([1000000, 1000002])
    detect_squares.add([1000002, 1000000])
    assert detect_squares.count([1000002, 1000002]) == 1


def test_multiple_squares(detect_squares):
    detect_squares.add([0, 1])
    detect_squares.add([1, 1])
    detect_squares.add([0, 2])
    detect_squares.add([1, 2])

    detect_squares.add([2, 2])
    detect_squares.add([2, 1])
    assert detect_squares.count([1, 2]) == 2  # Two squares sharing points
