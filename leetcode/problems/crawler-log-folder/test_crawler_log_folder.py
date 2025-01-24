from typing import Iterator, Union, Generator


class GoUp:
    pass


class ChDir:
    def __init__(self, dir_name: str):
        self.dir_name = dir_name


Token = Union[GoUp, ChDir]


class Solution:
    def parseTokens(self, logs: list[str]) -> Generator[Token, None, None]:
        for entry in logs:
            if entry.startswith(".."):
                yield GoUp()
            elif entry.startswith("."):
                continue
            else:
                yield ChDir(entry)
        return

    def processTokens(self, token_iterator: Iterator[Token]) -> int:
        stack = []
        for token in token_iterator:
            match token:
                case ChDir(dir_name=name):
                    stack.append(name)
                case GoUp():
                    if len(stack):
                        stack.pop()
        return len(stack)

    def minOperations(self, logs: list[str]) -> int:
        token_iter = self.parseTokens(logs)
        return self.processTokens(token_iter)


def test_leet1():
    logs = ["d1/", "d2/", "../", "d21/", "./"]
    output = 2
    res = Solution().minOperations(logs)
    assert res == output


def test_leet2():
    logs = ["d1/", "d2/", "./", "d3/", "../", "d31/"]
    output = 3
    res = Solution().minOperations(logs)
    assert res == output


def test_leet3():
    logs = ["d1/", "../", "../", "../"]
    output = 0
    res = Solution().minOperations(logs)
    assert res == output
