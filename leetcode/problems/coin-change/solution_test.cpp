#include <algorithm>
#include <gtest/gtest.h>

#include <limits>
#include <ranges>
#include <vector>
namespace rng = std::ranges;
namespace view = std::views;

namespace {
template <typename Range> auto enumerate(Range &&range) {
  return std::views::zip(std::views::iota(0), std::forward<Range>(range));
}
} // namespace

struct Solution {
  int coinChange(std::vector<int> &coins, int amount) {
    if (amount == 0) {
      return 0;
    }
    const auto kNone = std::numeric_limits<int>::max();
    std::vector<int> changeHistory;
    // Мы будем идти снизу вверх, заполняем сразу же массив сдачи
    changeHistory.resize(amount + 1, kNone);
    for (auto &&[target, minChange] : enumerate(changeHistory)) {
      // Пропускаем кейсы когда подходящих монет нет
      for (const auto coin : coins | view::filter([target](auto coin) {
                               return coin <= target;
                             })) {
        // Подсчитаем сколько мы не добираем сдачи, с учётом монеты "coin"
        auto diff = target - coin;
        // Особый случай - одной монеты достаточно, мы нашли идеальный ответ!
        if (diff == 0) {
          minChange = 1;
          break;
        }
        // Если же нет, то у нас есть два варианта:
        auto &&memoized = changeHistory[diff];
        // b) Информацию по diff нет, сдачу собрать невозможно
        if (memoized == kNone) {
          continue;
        }
        // a) Массив "change" хранит информацию о сдаче для разницы "diff"
        // Не забываем про нашу монету (+1)
        auto candidate = memoized + 1;
        minChange = std::min(minChange, candidate);
      }
    }
    return changeHistory.back() != kNone ? changeHistory.back() : -1;
  }
};

// Тесты
TEST(CoinChangeTest, BasicTests) {
  Solution solution;

  {
    SCOPED_TRACE("Пример 1: coins = [1, 2, 5], amount = 11");
    std::vector<int> coins = {1, 2, 5};
    EXPECT_EQ(solution.coinChange(coins, 11), 3);
  }

  {
    SCOPED_TRACE("Пример 2: coins = [2], amount = 3");
    std::vector<int> coins = {2};
    EXPECT_EQ(solution.coinChange(coins, 3), -1);
  }

  {
    SCOPED_TRACE("Пример 3: coins = [1], amount = 0");
    std::vector<int> coins = {1};
    EXPECT_EQ(solution.coinChange(coins, 0), 0);
  }
}

TEST(CoinChangeTest, EdgeCases) {
  Solution solution;

  {
    SCOPED_TRACE("Крайний случай 1: Нет монет, amount = 0");
    std::vector<int> coins = {};
    EXPECT_EQ(solution.coinChange(coins, 0), 0);
  }

  {
    SCOPED_TRACE("Крайний случай 2: Нет монет, amount = 5");
    std::vector<int> coins = {};
    EXPECT_EQ(solution.coinChange(coins, 5), -1);
  }

  {
    SCOPED_TRACE("Крайний случай 3: Одна монета равна amount");
    std::vector<int> coins = {5};
    EXPECT_EQ(solution.coinChange(coins, 5), 1);
  }

  {
    SCOPED_TRACE("Крайний случай 4: Одна монета больше amount");
    std::vector<int> coins = {10};
    EXPECT_EQ(solution.coinChange(coins, 5), -1);
  }

  {
    SCOPED_TRACE("Крайний случай 5: Монеты не могут составить amount");
    std::vector<int> coins = {3, 5};
    EXPECT_EQ(solution.coinChange(coins, 7), -1);
  }

  {
    SCOPED_TRACE("Крайний случай 6: Монеты могут составить amount");
    std::vector<int> coins = {1, 2, 5};
    EXPECT_EQ(solution.coinChange(coins, 6), 2); // 5 + 1
  }
}

TEST(CoinChangeTest, LargeAmount) {
  Solution solution;

  {
    SCOPED_TRACE("Большая сумма: coins = [1, 2, 5], amount = 100");
    std::vector<int> coins = {1, 2, 5};
    EXPECT_EQ(solution.coinChange(coins, 100), 20); // 20 монет по 5
  }
}

TEST(CoinChangeTest, LargeCoins) {
  Solution solution;

  {
    SCOPED_TRACE("Большие номиналы монет: coins = [1, 2, 5, 10, 20, 50, 100], "
                 "amount = 123");
    std::vector<int> coins = {1, 2, 5, 10, 20, 50, 100};
    EXPECT_EQ(solution.coinChange(coins, 123), 4); // 100 + 20 + 2 + 1
  }
}

TEST(CoinChangeTest, LeetCodeBugs) {
  Solution solution;

  {
    SCOPED_TRACE("Бага из leetcode");
    std::vector<int> coins = {186, 419, 83, 408};
    EXPECT_EQ(solution.coinChange(coins, 6249), 20);
  }
}