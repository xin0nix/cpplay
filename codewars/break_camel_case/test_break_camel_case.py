import pytest

from typing import Dict, Literal


def solution(text: str):
    trans_table: Dict[int, str] = dict()
    for code in range(ord("A"), ord("Z") + 1):
        trans_table[code] = f" {chr(code)}"
    return text.translate(trans_table).strip()


@pytest.mark.parametrize(
    "input_str, expected",
    [  # Provided examples
        ("camelCasing", "camel Casing"),
        ("identifier", "identifier"),
        ("", ""),
        # Single character
        ("a", "a"),
        ("A", "A"),
        # All lowercase
        ("lowercase", "lowercase"),
        # All uppercase (should not insert spaces according to examples)
        # Starts with uppercase, then lowercase
        ("CamelCase", "Camel Case"),
        # Camel case with multiple breaks
        ("thisIsCamelCase", "this Is Camel Case"),
        ("breakUpCamelCasing", "break Up Camel Casing"),
        # Edge: ends with uppercase
        ("someValueX", "some Value X"),
        # Edge: starts with uppercase, multiple splits
        ("CamelCaseTest", "Camel Case Test"),
    ],
)
def test_camel_case(input_str: str, expected: str):
    assert solution(input_str) == expected
