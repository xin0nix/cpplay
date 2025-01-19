#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ranges>
#include <string>
#include <unordered_map>

namespace rng = std::ranges;
namespace view = rng::views;

using std::operator""sv;

struct Solution {
  bool wordPattern(std::string &pattern, std::string &text) {
    // Разбить текст на отдельные слова
    auto words = text | view::split(" "sv);
    auto numWords =
        rng::fold_left(words, 0UL, [](auto acc, auto) { return acc + 1; });
    // Проверка "на дурачка"
    if (numWords != pattern.size()) {
      return false;
    }
    std::unordered_map<char, std::string_view> charsToWords{};
    std::unordered_map<std::string_view, char> wordsToChars{};
    for (auto &&[aChar, subText] : view::zip(pattern, words)) {
      std::string_view word(subText);
      if (auto charIt = charsToWords.find(aChar);
          charIt != charsToWords.cend()) {
        if (charIt->second != word) {
          return false;
        }
        // Тут всё в норме, мы нашли отображение и оно совпало
      } else {
        charsToWords[aChar] = word;
      }
      if (auto wordIt = wordsToChars.find(word);
          wordIt != wordsToChars.cend()) {
        if (wordIt->second != aChar) {
          return false;
        }
        // Тут всё в норме, мы нашли отображение и оно совпало
      } else {
        wordsToChars[word] = aChar;
      }
    }
    return true;
  }
};

TEST(WordPatternTest, LeetCode1) {
  Solution solution;
  std::string pattern = "abba";
  std::string text = "dog cat cat dog";
  ASSERT_TRUE(solution.wordPattern(pattern, text));
}

TEST(WordPatternTest, LeetCode2) {
  Solution solution;
  std::string pattern = "abba";
  std::string text = "dog cat cat fish";
  ASSERT_FALSE(solution.wordPattern(pattern, text));
}

TEST(WordPatternTest, Custom1) {
  Solution solution;
  std::string pattern = "abbf";
  std::string text = "dog cat cat dog";
  ASSERT_FALSE(solution.wordPattern(pattern, text));
}