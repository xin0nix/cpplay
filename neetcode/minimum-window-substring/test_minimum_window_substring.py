import pytest
from typing import Dict
from collections import defaultdict


class Solution:
    def minWindow(self, message: str, target: str) -> str:
        target_freqs: Dict[str, int] = defaultdict(int)
        window_freqs: Dict[str, int] = defaultdict(int)
        for letter in target:
            target_freqs[letter] += 1
        have = 0
        need = len(target_freqs)
        size = len(message)
        min_len = float("inf")
        res = ""
        left = 0
        right = 0
        while True:
            if have < need:
                if right == size:
                    break
                letter = message[right]
                window_freqs[letter] += 1
                right += 1
                if letter not in target_freqs:
                    continue
                if window_freqs[letter] == target_freqs[letter]:
                    have += 1
            else:
                window_len = right - left + 1
                if window_len < min_len:
                    res = message[left:right]
                    min_len = window_len
                letter = message[left]
                window_freqs[letter] -= 1
                left += 1
                if left == size:
                    break
                if letter not in target_freqs:
                    continue
                if window_freqs[letter] < target_freqs[letter]:
                    have -= 1

        return res


def min_window(s: str, t: str) -> str:
    return Solution().minWindow(s, t)


# Test cases inspired by LeetCode and problem explanations
@pytest.mark.parametrize(
    "s,t,expected",
    [
        ("ADOBECODEBANC", "ABC", "BANC"),  # Standard case with minimum substring
        ("a", "a", "a"),  # Single character match
        ("a", "aa", ""),  # Not enough characters in s
        ("aabdec", "abc", "abdec"),  # Minimum at the end
        ("abdabca", "abc", "abc"),  # Minimum in the middle
        ("ADOBECODEBANC", "XYZ", ""),  # Characters not found in s
        ("aa", "aa", "aa"),  # All of s is needed
        ("bba", "ab", "ba"),  # Minimum at the end
        ("ADOBECODEBANC", "AABC", "ADOBECODEBA"),  # Duplicates in t
    ],
)
def test_min_window(s, t, expected):
    assert min_window(s, t) == expected
