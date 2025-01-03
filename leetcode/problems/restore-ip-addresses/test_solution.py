from pytest_unordered import unordered


class Solution:
    def split(self, nums: str, level=0) -> list[list[int]] | None:
        """
        Перебираем все варианты разбиения на числа
        Невалидный результат это список пустых списков

        :param nums: Строка из цифр, из которых можно составлять числа - компоненты ip адреса
        :return:     Список списков цифр, образующих валидные фрагменты ip адреса
        """
        if nums == "":
            if level == 4:
                return [[]]
            return None
        if level > 4:
            return None
        # Не имеет смысла составлять голову из пустого списка
        start = 1
        # Не имеет смысла также её делать из более чем 3х цифр
        stop = min(len(nums), 3)
        # Вызываем split рекурсивно, разбивая nums на голову и хвостик
        res: list[list[int]] | None = None
        for i in range(start, stop + 1):
            head = nums[:i]
            head_num = int(head)
            # Голова может быть невалидной в двух случаях:
            # - Начинается с нуля, но нулём не является
            # - Выходит за диапазон [0, 255]
            if (head[0] == "0" and len(head) != 1) or head_num > 255:
                continue
            tail = self.split(nums[i:], level + 1)
            if tail is None:
                continue
            if res is None:
                res = []
            for t in tail:
                res.append([head_num] + t)
        return res

    def restoreIpAddresses(self, s: str) -> list[str]:
        """
        Восстанавливает все возможные валидные IP-адреса из зад анной строки цифр.

        Функция разбивает строку на 4 части, каждая из которых представляет собой
        валидный октет IP-адреса. Октеты должны быть в диапазоне от 0 до 255 и
        не могут начинаться с нуля, если это не единственный символ.

        :param s: Строка, содержащая только цифры (например, "25525511135").
        :return: Список строк, представляющих все возможные валидные IP-адреса.
                Если валидных IP-адресов нет, возвращает пустой список.
        """
        if len(s) < 4 or len(s) > 12:
            return []
        maybe_ips = self.split(s)
        if maybe_ips is None:
            return []
        res: list[str] = []
        for ip in maybe_ips:
            res.append(".".join(map(str, ip)))
        return res


def test_leet_1():
    s = "25525511135"
    expected = ["255.255.11.135", "255.255.111.35"]
    ips = Solution().restoreIpAddresses(s)
    assert ips == unordered(expected)


def test_leet_2():
    s = "0000"
    expected = ["0.0.0.0"]
    ips = Solution().restoreIpAddresses(s)
    assert ips == unordered(expected)


def test_leet_3():
    s = "102045"
    expected = ["1.0.20.45", "1.0.204.5", "10.2.0.45", "10.20.4.5", "102.0.4.5"]
    ips = Solution().restoreIpAddresses(s)
    assert ips == unordered(expected)


def test_leeg_bug_1():
    s = "010010"
    expected = ["0.10.0.10", "0.100.1.0"]
    ips = Solution().restoreIpAddresses(s)
    assert ips == unordered(expected)


def test_bad_1():
    s = ""
    expected = []
    ips = Solution().restoreIpAddresses(s)
    assert ips == unordered(expected)


def test_bad_2():
    s = "42"
    expected = []
    ips = Solution().restoreIpAddresses(s)
    assert ips == unordered(expected)


def test_corner_1():
    s = "100100100100"
    expected = ["100.100.100.100"]
    ips = Solution().restoreIpAddresses(s)
    assert ips == unordered(expected)


def test_corner_2():
    s = "255255255255"
    expected = ["255.255.255.255"]
    ips = Solution().restoreIpAddresses(s)
    assert ips == unordered(expected)
