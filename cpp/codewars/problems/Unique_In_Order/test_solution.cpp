#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace std::string_literals;

template <typename Iterator> auto uniqueInOrder(Iterator first, Iterator last) {
  using Item = std::decay_t<decltype(*std::declval<Iterator>())>;
  std::vector<Item> result;
  std::optional<Item> prev;
  for (; first != last; ++first) {
    if (!prev) {
      result.push_back(*first);
      prev = *first;
      continue;
    }
    if (*first != *prev) {
      result.push_back(*first);
    }
    prev = *first;
  }
  return result;
}

template <typename T>
std::vector<T> uniqueInOrder(const std::vector<T> &iterable) {
  return uniqueInOrder(iterable.begin(), iterable.end());
}

std::vector<char> uniqueInOrder(const std::string &iterable) {
  return uniqueInOrder(iterable.begin(), iterable.end());
}

TEST(UniqueInOrder, EmptyInput) {
  std::vector<char> empty_vec;
  EXPECT_THAT(uniqueInOrder(""s), ::testing::ElementsAre());
  EXPECT_THAT(uniqueInOrder(std::vector<int>{}), ::testing::ElementsAre());
}

TEST(UniqueInOrder, SingleElement) {
  EXPECT_THAT(uniqueInOrder(std::string("A")), std::vector<char>{'A'});
  EXPECT_THAT(uniqueInOrder(std::vector<int>{1}), std::vector<int>{1});
}

TEST(UniqueInOrder, ConsecutiveDuplicatesString) {
  EXPECT_THAT(uniqueInOrder(std::string("AA")), std::vector<char>{'A'});
  EXPECT_THAT(uniqueInOrder(std::string("AAAABBBCCDAABBB")),
              ::testing::ElementsAre('A', 'B', 'C', 'D', 'A', 'B'));
  EXPECT_THAT(uniqueInOrder(std::string("ABBCcAD")),
              ::testing::ElementsAre('A', 'B', 'C', 'c', 'A', 'D'));
  EXPECT_THAT(uniqueInOrder(std::string("AADD")),
              ::testing::ElementsAre('A', 'D'));
  EXPECT_THAT(uniqueInOrder(std::string("AAD")),
              ::testing::ElementsAre('A', 'D'));
  EXPECT_THAT(uniqueInOrder(std::string("ADD")),
              ::testing::ElementsAre('A', 'D'));
}

TEST(UniqueInOrder, ConsecutiveDuplicatesVector) {
  EXPECT_THAT(uniqueInOrder(std::vector<int>{1, 2, 2, 3, 3}),
              ::testing::ElementsAre(1, 2, 3));
  EXPECT_THAT(uniqueInOrder(std::vector<char>{'a', 'b', 'b'}),
              ::testing::ElementsAre('a', 'b'));
}

TEST(UniqueInOrder, MixedCases) {
  EXPECT_THAT(uniqueInOrder("ABBCcAD"s),
              ::testing::ElementsAre('A', 'B', 'C', 'c', 'A', 'D'));
}
