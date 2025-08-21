import pytest


def alphanumeric(password: str) -> bool:
    low_a = ord("a")
    low_z = ord("z")
    up_a = ord("A")
    up_z = ord("Z")
    di_0 = ord("0")
    di_9 = ord("9")
    seen = False
    for letter in password:
        p = ord(letter)
        lower = p >= low_a and p <= low_z
        uppder = p >= up_a and p <= up_z
        digit = p >= di_0 and p <= di_9
        if lower or uppder or digit:
            seen = True
            continue
        return False
    return seen


@pytest.mark.parametrize(
    "password, expected",
    [
        ("abc123", True),  # valid alphanumeric with letters and numbers
        ("A1B2C3", True),  # mixed upper/lower case letters and digits
        ("123456", True),  # digits only
        ("abcdef", True),  # letters only
        ("abc 123", False),  # contains space invalid
        ("abc_123", False),  # contains underscore invalid
        ("abc-123", False),  # contains invalid character "-"
        ("abc@123", False),  # contains invalid character "@"
        ("", False),  # only spaces invalid
        ("   ", False),  # only spaces invalid
    ],
)
def test_alphanumeric(password, expected):
    assert alphanumeric(password) == expected
