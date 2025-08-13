import pytest
from typing import List, Dict, Set, Deque, Tuple
from collections import defaultdict, deque


class Solution:
    def ladderLength(self, beginWord: str, endWord: str, wordList: List[str]) -> int:
        # Adjecency list
        adjList: Dict[str, Set[str]] = defaultdict(set)
        waitList: Deque[Tuple[int, str]] = deque()
        seen: Set[str] = set()

        def patternify(word):
            for n in range(len(word)):
                yield word[:n] + "*" + word[n + 1 :]

        def adjify(word):
            for entry in patternify(word):
                adjList[entry].add(word)

        adjify(beginWord)
        for word in wordList:
            adjify(word)

        waitList.append([1, beginWord])
        while waitList:
            [distance, word] = waitList.popleft()
            if word == endWord:
                return distance
            neighbours: Set[str] = set()
            for entry in patternify(word):
                for nei in adjList[entry]:
                    neighbours.add(nei)
            neighbours.discard(word)
            for nei in neighbours:
                if not nei in seen:
                    waitList.append([distance + 1, nei])
            seen.add(word)

        return 0


def ladderLength(beginWord: str, endWord: str, wordList: List[str]) -> int:
    return Solution().ladderLength(beginWord, endWord, wordList)


def test_basic_case():
    # Standard transformation, answer is 5: "hit" → "hot" → "dot" → "dog" → "cog"
    assert ladderLength("hit", "cog", ["hot", "dot", "dog", "lot", "log", "cog"]) == 5


def test_no_possible_path():
    # No connection to endWord ("cog" not in list)
    assert ladderLength("hit", "cog", ["hot", "dot", "dog", "lot", "log"]) == 0



# def test_begin_equals_end():
#     # start and end are the same word
#     assert ladderLength("hit", "hit", ["hit"]) == 0


def test_end_word_not_in_word_list():
    # If endWord not in list at all
    assert ladderLength("hit", "cog", ["hot", "dot", "dog", "lot", "log"]) == 0


def test_word_list_with_duplicates():
    # Duplicates in wordList should not affect answer
    assert (
        ladderLength("hit", "cog", ["hot", "dot", "dog", "lot", "log", "cog", "cog"])
        == 5
    )


def test_empty_word_list():
    # No transformations possible
    assert ladderLength("hit", "cog", []) == 0


# To run these tests, just call pytest from the command line if you have it installed.
