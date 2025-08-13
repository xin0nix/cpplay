import pytest
from typing import Dict, List


class Solution:
    def __init__(self) -> None:
        self.mapping: Dict[int, str] = {
            2: "abc",
            3: "def",
            4: "ghi",
            5: "jkl",
            6: "mno",
            7: "pqrs",
            8: "tuv",
            9: "wxyz",
        }
        self.result: List[str] = []

    def dfs(self, idx: int, combination: str):
        if idx == len(self.digits):
            self.result.append(combination)
            return
        num = int(self.digits[idx])
        letters = self.mapping[num]
        for letter in letters:
            self.dfs(idx + 1, combination + letter)

    def letterCombinations(self, digits: str) -> List[str]:
        if len(digits) == 0:
            return []
        self.digits: str = digits
        self.dfs(0, "")
        return self.result


# Example signature of the function to be tested
# def letter_combinations(digits: str) -> list[str]:
#     pass


@pytest.mark.parametrize(
    "digits, expected",
    [
        ("", []),  # empty input should return empty list
        ("2", ["a", "b", "c"]),  # single digit mapped to letters
        ("3", ["d", "e", "f"]),
        (
            "23",
            ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"],
        ),  # two digits combination
        ("9", ["w", "x", "y", "z"]),  # digit with 4 letters
        (
            "79",
            [
                "pw",
                "px",
                "py",
                "pz",
                "qw",
                "qx",
                "qy",
                "qz",
                "rw",
                "rx",
                "ry",
                "rz",
                "sw",
                "sx",
                "sy",
                "sz",
            ],
        ),  # digits both with 4 letters
        (
            "234",
            [
                "adg",
                "adh",
                "adi",
                "aeg",
                "aeh",
                "aei",
                "afg",
                "afh",
                "afi",
                "bdg",
                "bdh",
                "bdi",
                "beg",
                "beh",
                "bei",
                "bfg",
                "bfh",
                "bfi",
                "cdg",
                "cdh",
                "cdi",
                "ceg",
                "ceh",
                "cei",
                "cfg",
                "cfh",
                "cfi",
            ],
        ),  # three digits combination
    ],
)
def test_letter_combinations(digits, expected):
    result = Solution().letterCombinations(digits)
    # since output order can vary, compare as sets
    assert set(result) == set(expected)
