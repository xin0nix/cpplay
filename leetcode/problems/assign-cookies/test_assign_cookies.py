class Solution:
    def findContentChildren(self, greed: list[int], cookies: list[int]) -> int:
        greed.sort()
        cookies.sort()
        # kid будет хранить ответ, т.к. мы увеличиваем его только когда ребёнок съедает печенье
        # (менее жадные дети выигрывают, даже если мы могли накормить тем же печеньем более жадных)
        kid = 0
        idx = 0
        while kid < len(greed) and idx < len(cookies):
            # Если ребёнок слишком жадный - ищём следующее печенье
            # - все дети до него были менее жадными и уже получили печенье
            # - все дети после него будут либо такими же, либо ещё более жадными
            if greed[kid] > cookies[idx]:
                # Печенье больше никто не съест, сдвигаем idx вправо
                idx += 1
                continue
            # Ребёнок съел печенье, сдвигаем оба индекса вправо
            kid += 1
            idx += 1
        return kid


def test_1():
    g = [1, 2, 3]
    s = [1, 1]
    assert 1 == Solution().findContentChildren(g, s)


def test_1_opposite():
    g = [1, 1]
    s = [1, 2, 3]
    assert 2 == Solution().findContentChildren(g, s)


def test_2():
    g = [1, 2]
    s = [1, 2, 3]
    assert 2 == Solution().findContentChildren(g, s)


def test_2_opposite():
    g = [1, 2, 3]
    s = [1, 2]
    assert 2 == Solution().findContentChildren(g, s)


def test_3():
    g = [101, 102, 103]
    s = [1, 2, 3, 4, 5, 6, 6]
    assert 0 == Solution().findContentChildren(g, s)


def test_3_opposite():
    g = [1, 2, 3, 4, 5, 6, 6]
    s = [101, 102, 103]
    assert 3 == Solution().findContentChildren(g, s)


def test_4():
    g = [101, 102, 103, 105, 106]
    s = [1, 2, 3]
    assert 0 == Solution().findContentChildren(g, s)


def test_4_opposite():
    g = [1, 2, 3, 4]
    s = [101, 102, 103, 105, 105, 106]
    assert 4 == Solution().findContentChildren(g, s)


def test_5():
    g = [3, 3, 3, 3, 3]
    s = [4, 4, 4]
    assert 3 == Solution().findContentChildren(g, s)


def test_5_opposite():
    g = [4, 4, 4]
    s = [3, 3, 3, 3, 3]
    assert 0 == Solution().findContentChildren(g, s)
