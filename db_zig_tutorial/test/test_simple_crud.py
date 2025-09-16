import pytest
import subprocess


@pytest.fixture
def run_script():
    def _run_script(commands):
        # Use shell=True so `zig build run` runs properly
        process = subprocess.Popen(
            "zig build run",
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            shell=True,
        )
        input_str = "\n".join(commands) + "\n"
        output, _ = process.communicate(input=input_str)
        return output.splitlines()

    return _run_script


def test_inserts_and_retrieves_row(run_script):
    result = run_script(
        [
            "insert 1 user1 person1@example.com",
            "select",
            ".exit",
        ]
    )
    expected = [
        "(1, user1, person1@example.com)",
        "bye...",
    ]
    assert result == expected


def test_prints_error_message_when_table_is_full(run_script):
    script = [f"insert {i} user{i} person{i}@example.com" for i in range(0, 141)]
    script.append(".exit")
    result = run_script(script)
    assert result[-2] == "Out of range!"


def test_allows_inserting_strings_that_are_maximum_length(run_script):
    long_username = "a" * 31
    long_email = "a" * 255
    script = [
        f"insert 1 {long_username} {long_email}",
        "select",
        ".exit",
    ]
    result = run_script(script)
    expected = [
        f"(1, {long_username}, {long_email})",
        "bye...",
    ]
    assert result == expected


def test_prints_error_message_if_username_is_too_long(run_script):
    long_username = "a" * 32
    long_email = "a" * 255
    script = [
        f"insert 1 {long_username} {long_email}",
        "select",
        ".exit",
    ]
    result = run_script(script)
    expected = [
        f"(1, {long_username}, {long_email})",
        "bye...",
    ]
    assert result[0] == "Token exceeds field capacity: 32 >= 32"


def test_prints_error_message_if_email_is_too_long(run_script):
    long_username = "a" * 31
    long_email = "a" * 256
    script = [
        f"insert 1 {long_username} {long_email}",
        "select",
        ".exit",
    ]
    result = run_script(script)
    expected = [
        f"(1, {long_username}, {long_email})",
        "bye...",
    ]
    assert result[0] == "Token exceeds field capacity: 256 >= 256"
