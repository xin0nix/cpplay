#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ranges>
#include <string_view>
#include <variant>
namespace rng = std::ranges;
namespace view = std::views;

struct Solution {
  struct GoDown {};
  struct Stay {};
  struct GoUp {
    std::string_view dirName;
  };
  using Token = std::variant<Stay, GoDown, GoUp>;
  int minOperations(std::vector<std::string> &logs) {
    auto tokenized =
        logs | view::transform([](std::string_view token) -> Token {
          if (token.starts_with("..")) {
            return GoDown{};
          }
          if (token.starts_with(".")) {
            return Stay{};
          }
          return GoUp{.dirName = token};
        });
    return rng::fold_left(tokenized, int{}, [](auto stackSize, auto token) {
      if (std::holds_alternative<GoDown>(token)) {
        return std::max(0, stackSize - 1);
      }
      if (std::holds_alternative<Stay>(token)) {
        return stackSize;
      }
      assert(std::holds_alternative<GoUp>(token));
      return stackSize + 1;
    });
  }
};

struct CrawlerLogFolderTest : ::testing::Test {};

TEST_F(CrawlerLogFolderTest, Tests) {
  Solution solution;
  using std::string;
  using std::vector;

  {
    SCOPED_TRACE("Тест 1: Простой случай с перемещением вглубь и возвратом на "
                 "один уровень вверх.");
    vector<string> logs = {"d1/", "d2/", "../", "d21/", "./"};
    EXPECT_EQ(solution.minOperations(logs), 2);
  }

  {
    SCOPED_TRACE("Тест 2: Перемещение вглубь, текущий каталог, затем вглубь и "
                 "возврат на один уровень вверх.");
    vector<string> logs = {"d1/", "d2/", "./", "d3/", "../", "d31/"};
    EXPECT_EQ(solution.minOperations(logs), 3);
  }

  {
    SCOPED_TRACE("Тест 3: Попытка выйти за пределы корневого каталога.");
    vector<string> logs = {"d1/", "../", "../", "../"};
    EXPECT_EQ(solution.minOperations(logs), 0);
  }

  {
    SCOPED_TRACE("Тест 4: Многократное использование текущего каталога.");
    vector<string> logs = {"d1/", "./", "./", "d2/", "./", "d3/", "./"};
    EXPECT_EQ(solution.minOperations(logs), 3);
  }

  {
    SCOPED_TRACE("Тест 5: Перемещение вглубь и возврат на два уровня вверх "
                 "(разбито на две операции).");
    vector<string> logs = {"d1/", "d2/", "d3/", "../", "../", "d4/"};
    EXPECT_EQ(solution.minOperations(logs), 2);
  }

  {
    SCOPED_TRACE("Тест 6: Перемещение вглубь, возврат на два уровня вверх "
                 "(разбито на две операции) и затем вглубь.");
    vector<string> logs = {"d1/", "d2/", "../", "../", "d3/"};
    EXPECT_EQ(solution.minOperations(logs), 1);
  }

  {
    SCOPED_TRACE("Тест 7: Комбинация перемещений вглубь и возвратов.");
    vector<string> logs = {"d1/", "../", "d2/", "./", "d3/", "../", "d4/"};
    EXPECT_EQ(solution.minOperations(logs), 2);
  }

  {
    SCOPED_TRACE("Тест 8: Многократное использование текущего каталога.");
    vector<string> logs = {"./", "./", "./", "./"};
    EXPECT_EQ(solution.minOperations(logs), 0);
  }

  {
    SCOPED_TRACE("Тест 9: Попытка выйти за пределы корневого каталога.");
    vector<string> logs = {"../", "../", "../"};
    EXPECT_EQ(solution.minOperations(logs), 0);
  }

  {
    SCOPED_TRACE("Тест 10: Перемещение вглубь на несколько уровней и возврат "
                 "на несколько уровней вверх (разбито на четыре операции).");
    vector<string> logs = {"d1/", "d2/", "d3/", "d4/", "d5/",
                           "../", "../", "../", "../", "d6/"};
    EXPECT_EQ(solution.minOperations(logs), 2);
  }
}