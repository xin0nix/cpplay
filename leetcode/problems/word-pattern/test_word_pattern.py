class Solution:
    def wordPattern(self, pattern: str, text: str) -> bool:
        words: list[str] = text.split(" ")
        if len(pattern) != len(words):
            return False
        chars_to_words = dict()
        words_to_chars = dict()
        for [char, word] in zip(pattern, words):
            if char in chars_to_words and word != chars_to_words[char]:
                return False
            else:
                chars_to_words[char] = word
            if word in words_to_chars and char != words_to_chars[word]:
                return False
            else:
                words_to_chars[word] = char
        return True


def test_leet1():
    pattern = "abba"
    s = "dog cat cat dog"
    assert Solution().wordPattern(pattern, s)

def test_leet2():
    pattern = "abba"
    s = "dog cat cat fish"
    assert not Solution().wordPattern(pattern, s)

def test_custom1():
    pattern = "abbf"
    s = "dog cat cat dog"
    assert not Solution().wordPattern(pattern, s)