import pytest


class Solution:
    def multiply(self, num1: str, num2: str) -> str:
        if "0" in [num1, num2]:
            return "0"

        product = [0] * (len(num1) + len(num2))

        for i1, v1 in enumerate(num1[::-1]):
            for i2, v2 in enumerate(num2[::-1]):
                product[i1 + i2] += int(v1) * int(v2)  # might be >= 10!
                product[i1 + i2 + 1] += product[i1 + i2] // 10
                product[i1 + i2] %= 10

        prod_str = "".join(map(str, product))
        return prod_str[::-1].lstrip("0")


@pytest.mark.parametrize(
    "num1, num2, expected",
    [
        # Basic tests
        ("2", "3", "6"),
        ("456", "123", "56088"),
        # Multiplying by zero
        ("0", "12345", "0"),
        ("12345", "0", "0"),
        ("0", "0", "0"),
        # Single digit multiplication
        ("9", "9", "81"),
        ("5", "4", "20"),
        # Multiplying numbers where one is '1' (identity)
        ("1", "99999", "99999"),
        ("99999", "1", "99999"),
        # Large numbers multiplication to test performance
        ("123456789", "987654321", "121932631112635269"),
        # Larger inputs to test correctness
        (
            "999999999999999999",
            "999999999999999999",
            "999999999999999998000000000000000001",
        ),
    ],
)
def test_multiply_strings(num1, num2, expected):
    result = Solution().multiply(num1, num2)
    assert (
        result == expected
    ), f"Expected {expected} but got {result} for inputs {num1} and {num2}"
