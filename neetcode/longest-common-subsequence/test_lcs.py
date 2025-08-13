import unittest


class Solution:
    def longestCommonSubsequence(self, text1: str, text2: str) -> int:
        n1 = len(text1)
        n2 = len(text2)
        grid = [[0 for _ in range(n2 + 1)] for _ in range(n1 + 1)]
        for row in range(n1 - 1, -1, -1):
            for col in range(n2 - 1, -1, -1):
                if text1[row] == text2[col]:
                    grid[row][col] = 1 + grid[row + 1][col + 1]
                    continue
                below = grid[row + 1][col]
                right = grid[row][col + 1]
                grid[row][col] = max(below, right)
        return grid[0][0]


class TestLongestCommonSubsequence(unittest.TestCase):

    def setUp(self):
        self.sol = Solution()

    def test_example1(self):
        # "abcde" and "ace" => LCS is "ace"
        self.assertEqual(self.sol.longestCommonSubsequence("abcde", "ace"), 3)

    def test_example2(self):
        # "abc" and "abc" => LCS is "abc"
        self.assertEqual(self.sol.longestCommonSubsequence("abc", "abc"), 3)

    def test_example3(self):
        # "abc" and "def" => No common subsequence
        self.assertEqual(self.sol.longestCommonSubsequence("abc", "def"), 0)

    def test_partial_overlap(self):
        # "AGGTAB" and "GXTXAYB" => LCS is "GTAB" (length 4)
        self.assertEqual(self.sol.longestCommonSubsequence("AGGTAB", "GXTXAYB"), 4)

    def test_single_char_match(self):
        # "abc" and "cba" => LCS of length 1 (any single common character)
        self.assertEqual(self.sol.longestCommonSubsequence("abc", "cba"), 1)

    def test_empty_strings(self):
        # Both strings empty => LCS is 0
        self.assertEqual(self.sol.longestCommonSubsequence("", ""), 0)

    def test_one_empty(self):
        # One string empty
        self.assertEqual(self.sol.longestCommonSubsequence("abc", ""), 0)
        self.assertEqual(self.sol.longestCommonSubsequence("", "abc"), 0)

    def test_long_identical_strings(self):
        # Identical long strings
        self.assertEqual(self.sol.longestCommonSubsequence("abcdxyz", "abcdxyz"), 7)

    def test_subsequence_at_end(self):
        # LCS at the end
        self.assertEqual(self.sol.longestCommonSubsequence("xyzabcd", "abcd"), 4)


if __name__ == "__main__":
    unittest.main()
