import pytest
from typing import Set


def longest(a1: str, a2: str):
    every_letter = set(a1).union(a2)
    result = ""
    for letter in "abcdefghijklmnopqrstuvwxyz":
        if letter in every_letter:
            result += letter
    return result


@pytest.mark.parametrize(
    "a1,a2,expected",
    [
        ("abc", "def", "abcdef"),
        ("aab", "bbcc", "abc"),
        ("xyaabbbccccdefww", "xxxxyyyyabklmopq", "abcdefklmopqwxy"),
        (
            "abcdefghijklmnopqrstuvwxyz",
            "abcdefghijklmnopqrstuvwxyz",
            "abcdefghijklmnopqrstuvwxyz",
        ),
        ("", "abcd", "abcd"),
        ("xyz", "abc", "abcxyz"),
        ("aabbcc", "aabbcc", "abc"),
        ("a", "b", "ab"),
        ("a", "a", "a"),
    ],
)
def test_longest(a1, a2, expected):
    assert longest(a1, a2) == expected
