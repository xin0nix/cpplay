#include <functional>
#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class WordDictionary {
  struct Node {
    std::unordered_map<char, Node> children;
    bool terminal{false};
  };

  Node root;

public:
  WordDictionary() {}

  void addWord(const string &word) {
    if (word.empty()) {
      return;
    }
    using Iter = decltype(word.cbegin());
    auto *curNode = &root;
    std::function<void(Iter)> add = [&](Iter letterIt) {
      if (letterIt == word.cend()) {
        return;
      }
      // Find a child with an appropriate letter
      auto nodeIt = curNode->children.find(*letterIt);
      // If there is no child - add a new one and replace cur with it
      if (nodeIt == curNode->children.cend()) {
        auto newNode = curNode->children.emplace(*letterIt, Node{});
        // curNode->children[*letterIt] = Node{};
        curNode = &newNode.first->second;
      }
      // Otherwise - replace cur node with the child and repeat
      else {
        curNode = &nodeIt->second;
      }
      add(letterIt + 1);
    };
    add(word.begin());
    curNode->terminal = true;
  }

  bool search(const string &word) {
    if (word.empty()) {
      return true;
    }
    using Iter = decltype(word.cbegin());
    std::function<bool(Iter, Node *)> dfs = [&](Iter letterIt, Node *node) {
      // FIXME: handle terminal nodes
      if (letterIt == word.cend()) {
        return node->terminal;
      }
      if (*letterIt == '.') {
        // If letter is a wildcard '.'
        for (auto &&child : node->children) {
          if (dfs(letterIt + 1, &child.second)) {
            return true;
          }
        }
        return false;
      }
      // We need to visit all the children
      // Otherwise - find the only one that matches perfectly
      auto nodeIt = node->children.find(*letterIt);
      if (nodeIt == node->children.cend()) {
        return false;
      }
      return dfs(letterIt + 1, &nodeIt->second);
    };
    return dfs(word.begin(), &root);
  }
};

struct WordDictionaryTestParam {
  vector<pair<string, string>> operations; // {operation, word}
  vector<bool> expectedResults;            // Expected results for search
};

class WordDictionaryParamTest
    : public ::testing::TestWithParam<WordDictionaryTestParam> {};

TEST_P(WordDictionaryParamTest, BasicAndWildcard) {
  const auto &param = GetParam();
  WordDictionary dict;
  auto resultIt = param.expectedResults.cbegin();
  for (const auto &op : param.operations) {
    if (op.first == "add") {
      dict.addWord(op.second);
    } else if (op.first == "search") {
      bool found = dict.search(op.second);
      ASSERT_NE(resultIt, param.expectedResults.cend());
      ASSERT_EQ(found, *resultIt)
          << "Failed on search(\"" << op.second << "\")";
      ++resultIt;
    }
  }
}

// Test cases
INSTANTIATE_TEST_SUITE_P(
    WordDictionaryTests, WordDictionaryParamTest,
    ::testing::Values(
        // Basic add/search
        WordDictionaryTestParam{{{"add", "bad"},
                                 {"add", "dad"},
                                 {"add", "mad"},
                                 {"search", "pad"},
                                 {"search", "bad"},
                                 {"search", ".ad"},
                                 {"search", "b.."}},
                                {false, true, true, true}},
        // Empty string search
        WordDictionaryTestParam{{{"add", ""}, {"search", ""}}, {true}},
        // Wildcard only
        WordDictionaryTestParam{{{"add", "a"}, {"search", "."}}, {true}},
        // No words added
        WordDictionaryTestParam{{{"search", "a"}}, {false}},
        // Multiple identical words
        WordDictionaryTestParam{{{"add", "test"},
                                 {"add", "test"},
                                 {"search", "test"},
                                 {"search", "t.st"}},
                                {true, true}},
        // Prefix and full word
        WordDictionaryTestParam{
            {{"add", "apple"}, {"search", "app"}, {"search", "apple"}},
            {false, true}}));
