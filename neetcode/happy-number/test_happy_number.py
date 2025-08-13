import pytest


class Solution:

    def getNext(self, num: int):
        result: int = 0
        while num > 0:
            digit = num % 10
            result += digit * digit
            num = num // 10
        return result

    def isHappy(self, num: int) -> bool:
        slow = num
        fast = self.getNext(num)
        while slow != fast:
            slow = self.getNext(slow)
            if slow == 1:
                return True
            fast = self.getNext(self.getNext(fast))
        return slow == 1


@pytest.mark.parametrize(
    "n, expected",
    [
        (19, True),  # example from problem, known happy number
        (1, True),  # trivial happy number
        (7, True),  # another known happy number
        (2, False),  # known unhappy number
        (3, False),  # known unhappy number
        (4, False),  # unhappy number, leads to a cycle
        (20, False),  # example unhappy number from discussions
        (100, True),  # trivial, leads to 1 quickly
        (10, True),  # leads to 1 as well
        (111, False),  # not happy, cycles
        (44, True),  #  happy
    ],
)
def test_is_happy(n, expected):
    assert Solution().isHappy(n) == expected
