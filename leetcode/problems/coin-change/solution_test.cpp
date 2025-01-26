#include <algorithm>
#include <gtest/gtest.h>

#include <limits>
#include <vector>

struct Solution {
  int coinChange(std::vector<int> &coins, int amount) {
    if (amount == 0) {
      return 0;
    }
    const auto kNone = std::numeric_limits<int>::max();
    std::vector<int> change(amount + 1, kNone);
    // Если diff будет равен нулю (ниже), у нас будет обращение к 0му элементу
    // Мы будем идти снизу вверх, заполняем сразу же массив сдачи
    change.front() = 0;
    for (auto target = 0UL, limit = change.size(); target < limit; ++target) {
      auto &minChange = change.at(target);
      // Пропускаем кейсы когда подходящих монет нет
      for (const auto coin : coins) {
        if (target < coin) [[unlikely]] {
          continue;
        }
        // Подсчитаем сколько мы не добираем сдачи, с учётом выбранной монеты
        auto diff = target - coin;
        // Если же нет, то у нас есть два варианта:
        auto &&memoized = change[diff];
        // a) Информацию по diff нет, сдачу собрать невозможно
        if (memoized == kNone) {
          continue;
        }
        // b) Массив "change" хранит информацию о сдаче для разницы "diff"
        // Не забываем про нашу монету (+1)
        minChange = std::min(minChange, memoized + 1);
      }
    }
    return change.back() != kNone ? change.back() : -1;
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