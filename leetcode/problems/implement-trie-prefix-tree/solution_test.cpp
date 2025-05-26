/**
 * Solution: https://www.youtube.com/watch?v=oobqoCJlHA0
 * Task: https://leetcode.com/problems/implement-trie-prefix-tree/description/
 * Copy: https://neetcode.io/problems/implement-prefix-tree
 * Holst: -
 */

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

struct Trie {
  struct Node {
    using NodePtr = std::unique_ptr<Node>;
    std::array<NodePtr, 26> children = {nullptr};
    bool endOfWord{false};
  };

  Trie() { root = std::make_unique<Node>(); }

  int indexOf(char letter) { return letter - 'a'; }

  void insert(const string &word) {
    if (word.empty()) {
      return;
    }
    auto *node = root.get();
    for (auto letter : word) {
      auto letterIdx = indexOf(letter);
      if (auto &&child = node->children.at(letterIdx); child != nullptr) {
        node = child.get();
        continue;
      }
      node->children.at(letterIdx) = std::make_unique<Node>();
      node = node->children.at(letterIdx).get();
    }
    node->endOfWord = true;
  }

  bool search(const string &word) {
    auto *node = startsWithNode(word);
    if (node == nullptr) {
      return false;
    }
    return node->endOfWord;
  }

  bool startsWith(const string &prefix) {
    auto *node = startsWithNode(prefix);
    return node != nullptr;
  }

  Node *startsWithNode(const string &prefix) {
    if (prefix.empty()) {
      return root.get();
    }
    auto *node = root.get();
    for (auto letter : prefix) {
      auto letterIdx = indexOf(letter);
      if (auto &&child = node->children.at(letterIdx); child != nullptr) {
        node = child.get();
        continue;
      }
      return nullptr;
    }
    return node;
  }

  Node::NodePtr root;
};

struct TestCase {
  vector<tuple<string, string, bool>> operations;
};

class TrieTest : public ::testing::TestWithParam<TestCase> {};

TEST_P(TrieTest, TestTrieOperations) {
  Trie trie;
  for (const auto &op : GetParam().operations) {
    string operation = get<0>(op);
    string str = get<1>(op);
    bool expected = get<2>(op);
    if (operation == "insert") {
      trie.insert(str);
    } else if (operation == "search") {
      ASSERT_EQ(trie.search(str), expected);
    } else if (operation == "startsWith") {
      ASSERT_EQ(trie.startsWith(str), expected);
    } else {
      FAIL() << "Unknown operation: " << operation;
    }
  }
}

INSTANTIATE_TEST_SUITE_P(TrieTestCases, TrieTest,
                         ::testing::Values(
                             // Original first test case
                             TestCase{{{"insert", "apple", false},
                                       {"search", "apple", true},
                                       {"search", "app", false},
                                       {"startsWith", "app", true},
                                       {"startsWith", "", true}}},
                             // New Test Case 6: Diverging branches
                             TestCase{{{"insert", "apple", false},
                                       {"insert", "apluse", false},
                                       {"insert", "ape", false},
                                       {"search", "apple", true},
                                       {"search", "apluse", true},
                                       {"search", "ape", true},
                                       {"startsWith", "apl", true},
                                       {"startsWith", "ap", true},
                                       {"search", "apl", false},
                                       {"startsWith", "apples", false},
                                       {"startsWith", "apex", false}}},
                             // New Test Case 7: Single-letter words
                             TestCase{{{"insert", "a", false},
                                       {"insert", "b", false},
                                       {"insert", "c", false},
                                       {"insert", "d", false},
                                       {"insert", "e", false},
                                       {"insert", "f", false},
                                       {"insert", "g", false},
                                       {"search", "a", true},
                                       {"search", "g", true},
                                       {"search", "h", false},
                                       {"startsWith", "a", true},
                                       {"startsWith", "b", true},
                                       {"startsWith", "", true},
                                       {"startsWith", "x", false}}},
                             // New Test Case 8: Long overlapping words
                             TestCase{{{"insert", "abcdefgh", false},
                                       {"insert", "abcdxyz", false},
                                       {"search", "abcdefgh", true},
                                       {"search", "abcdxyz", true},
                                       {"search", "abcdefg", false},
                                       {"startsWith", "abcd", true},
                                       {"startsWith", "abcde", true},
                                       {"startsWith", "abcdx", true},
                                       {"startsWith", "abcdf", false}}},
                             // New Test Case 9: Duplicate insertions
                             TestCase{{{"insert", "apple", false},
                                       {"insert", "apple", false},
                                       {"search", "apple", true},
                                       {"startsWith", "app", true},
                                       {"search", "app", false}}}));
