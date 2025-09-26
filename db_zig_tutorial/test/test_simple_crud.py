import pytest
import subprocess
import tempfile


@pytest.fixture
def run_script():
    def _run_script(db_file, idx_file, commands):
        process = subprocess.Popen(
            f"zig build run -- {db_file.name} {idx_file.name}",
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
    with tempfile.NamedTemporaryFile(mode="w", delete=True) as db_file:
        with tempfile.NamedTemporaryFile(mode="w", delete=True) as idx_file:
            result = run_script(
                db_file,
                idx_file,
                [
                    "insert 1 user1 person1@example.com",
                    "select",
                    ".exit",
                ],
            )
            expected = [
                "(1, user1, person1@example.com)",
                "bye...",
            ]
            assert result == expected


def test_prints_error_message_when_table_is_full(run_script):
    script = [f"insert {i} user{i} person{i}@example.com" for i in range(0, 141)]
    script.append(".exit")
    with tempfile.NamedTemporaryFile(mode="w", delete=True) as db_file:
        with tempfile.NamedTemporaryFile(mode="w", delete=True) as idx_file:
            result = run_script(db_file, idx_file, script)
            assert result[-2] == "Out of range!"


def test_allows_inserting_strings_that_are_maximum_length(run_script):
    long_username = "a" * 31
    long_email = "a" * 255
    script = [
        f"insert 1 {long_username} {long_email}",
        "select",
        ".exit",
    ]
    with tempfile.NamedTemporaryFile(mode="w", delete=True) as db_file:
        with tempfile.NamedTemporaryFile(mode="w", delete=True) as idx_file:
            result = run_script(db_file, idx_file, script)
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
    with tempfile.NamedTemporaryFile(mode="w", delete=True) as db_file:
        with tempfile.NamedTemporaryFile(mode="w", delete=True) as idx_file:
            result = run_script(db_file, idx_file, script)
            assert result[0] == "Token exceeds field capacity: 32 >= 32"


def test_prints_error_message_if_email_is_too_long(run_script):
    long_username = "a" * 31
    long_email = "a" * 256
    script = [
        f"insert 1 {long_username} {long_email}",
        "select",
        ".exit",
    ]
    with tempfile.NamedTemporaryFile(mode="w", delete=True) as db_file:
        with tempfile.NamedTemporaryFile(mode="w", delete=True) as idx_file:
            result = run_script(db_file, idx_file, script)
            assert result[0] == "Token exceeds field capacity: 256 >= 256"


def test_persistency(run_script):
    with tempfile.NamedTemporaryFile(mode="w", delete=True) as db_file:
        with tempfile.NamedTemporaryFile(mode="w", delete=True) as idx_file:
            result = run_script(
                db_file,
                idx_file,
                [
                    "insert 1 user1 person1@example.com",
                    ".exit",
                ],
            )
            expected = [
                "bye...",
            ]
            assert result == expected
            result = run_script(
                db_file,
                idx_file,
                [
                    "select",
                    ".exit",
                ],
            )
            expected = [
                "(1, user1, person1@example.com)",
                "bye...",
            ]
            assert result == expected


def test_persistency_multiple_pages(run_script):
    insert = [f"insert {i} user{i} person{i}@example.com" for i in range(0, 140)]
    insert.append(".exit")

    with tempfile.NamedTemporaryFile(mode="w", delete=True) as db_file:
        with tempfile.NamedTemporaryFile(mode="w", delete=True) as idx_file:
            result = run_script(
                db_file,
                idx_file,
                insert,
            )
            expected = [
                "bye...",
            ]
            assert result == expected
            result = run_script(
                db_file,
                idx_file,
                [
                    "select",
                    ".exit",
                ],
            )
            for i in range(0, 140):
                assert result[i] == f"({i}, user{i}, person{i}@example.com)"
