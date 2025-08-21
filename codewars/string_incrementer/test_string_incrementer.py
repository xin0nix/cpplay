import pytest
import re

# https://www.codewars.com/kata/54a91a4883a7de5d7800009c/train/python


def increment_string(text: str):
    matches = re.findall("(\\d+)", text)
    if not matches:
        return text + "1"
    tail = matches[-1]
    if not text.endswith(tail):
        return text + "1"
    prefix = text[: len(text) - len(tail)]
    suffix = str(int(tail) + 1).zfill(len(tail))
    return prefix + suffix


@pytest.mark.parametrize(
    "input_str, expected",
    [
        ("foo", "foo1"),
        ("foobar23", "foobar24"),
        ("foo0042", "foo0043"),
        ("foo9", "foo10"),
        ("foo099", "foo100"),
        ("", "1"),  # Edge case: empty string
        ("foobar001", "foobar002"),
        ("foobar0099", "foobar0100"),
        ("foobar000", "foobar001"),
        ("foobar", "foobar1"),
        ("123", "124"),  # All digits case
        ("foo099bar", "foo099bar1"),  # number in middle, not at end
    ],
)
def test_increment_string(input_str, expected):
    assert increment_string(input_str) == expected
