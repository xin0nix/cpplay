class Solution:
    def coinChange(self, coins: list[int], amount: int) -> int:
        if amount == 0:
            return 0
        # Мы будем идти снизу вверх, заполняем сразу же массив сдачи (все None)
        change: list[None | int] = [None] * (amount + 1)
        for target in range(0, amount + 1):
            # Пропускаем кейсы когда подходящих монет нет (останется None)
            min_change: None | int = None  # аналогично change[target], кек :)
            for c in filter(lambda c: c <= target, coins):
                # Подсчитаем сколько мы не добираем сдачи, с учётом монеты "c"
                diff = target - c
                # Особый случай - одной монеты хватает, мы нашли идеальный ответ!
                if diff == 0:
                    min_change = 1
                    break
                # Если же нет, то у нас есть два варианта:
                memoized = change[diff]
                # a) Массив "change" хранит информацию о сдаче для разницы "diff"
                if memoized is not None:
                    # Не забываем про нашу монету
                    candidate = memoized + 1
                    if min_change is not None:
                        min_change = min(min_change, candidate)
                    else:
                        min_change = candidate
                # b) Такой информации нет (стоит None), сдачу собрать невозможно
            change[target] = min_change

        result = change[amount]
        if result is not None:
            return result
        return -1


def test_leet1():
    coins = [1, 2, 5]
    amount = 11
    output = 3
    assert output == Solution().coinChange(coins, amount)


def test_leet2():
    coins = [2]
    amount = 3
    output = -1
    assert output == Solution().coinChange(coins, amount)


def test_leet3():
    coins = [1]
    amount = 0
    output = 0
    assert output == Solution().coinChange(coins, amount)
