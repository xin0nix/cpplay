import pytest

from typing import List


def is_pangram(text: str):
    seen: List[int] = [0] * 26
    count: int = 0
    for letter in text:
        index = ord(letter.lower()) - ord("a")
        if index < 0 or index > 25:
            continue
        if seen[index] == 0:
            count += 1
            seen[index] = 1
    return count == 26


@pytest.mark.parametrize(
    "test_input,expected",
    [
        ("The quick brown fox jumps over the lazy dog", True),
        ("The quick brown fox jumps over the lazy do", False),  # missing 'g'
        ("Sphinx of black quartz, judge my vow!", True),  # pangram with punctuation
        ("Hello, world!", False),
        ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", True),  # uppercase alphabet only
        ("abcdefghijklmnopqrstuvwxyz", True),  # lowercase alphabet only
        ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", True),
        ("ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxy", False),
        ("", False),  # empty string
        ("1234567890!@#$%^&*()", False),  # only numbers and punctuation
        (
            "The quick brown fox jumps over the lazy dog 12345.",
            True,
        ),  # pangram with numbers
    ],
)
def test_is_pangram(test_input, expected):
    assert is_pangram(test_input) == expected
