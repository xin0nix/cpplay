import pytest

from collections import defaultdict
from functools import cmp_to_key


def comparator(a, b):
    if len(a) != len(b):
        return len(b) - len(a)
    if a < b:
        return -1
    if a > b:
        return 1
    return 0


def mix(s1: str, s2: str):
    f1 = defaultdict(int)
    f2 = defaultdict(int)
    for s in s1:
        f1[s] += 1
    for s in s2:
        f2[s] += 1
    res = []
    for o in range(ord("a"), ord("z") + 1):
        letter = chr(o)
        count1, count2 = f1[letter], f2[letter]
        m = max(count1, count2)
        if m <= 1:
            continue
        temp = ""
        if count1 == count2:
            temp += "="
        elif count1 > count2:
            temp += "1"
        else:
            temp += "2"
        temp += ":" + letter * m
        res.append(temp)
    res = sorted(res, key=cmp_to_key(comparator))
    return "/".join(res)


@pytest.mark.parametrize(
    "s1, s2, expected",
    [
        ("Are they here", "yes, they are here", "2:eeeee/2:yy/=:hh/=:rr"),
        (
            "Sadus:cpms>orqn3zecwGvnznSgacs",
            "MynwdKizfd$lvse+gnbaGydxyXzayp",
            "2:yyyy/1:ccc/1:nnn/1:sss/2:ddd/=:aa/=:zz",
        ),
        (
            "looping is fun but dangerous",
            "less dangerous than coding",
            "1:ooo/1:uuu/2:sss/=:nnn/1:ii/2:aa/2:dd/2:ee/=:gg",
        ),
        (
            " In many languages",
            " there's a pair of functions",
            "1:aaa/1:nnn/1:gg/2:ee/2:ff/2:ii/2:oo/2:rr/2:ss/2:tt",
        ),
        ("Lords of the Fallen", "gamekult", "1:ee/1:ll/1:oo"),
        ("codewars", "codewars", ""),
        (
            "A generation must confront the looming ",
            "codewarrs",
            "1:nnnnn/1:ooooo/1:tttt/1:eee/1:gg/1:ii/1:mm/=:rr",
        ),
    ],
)
def test_mix(s1, s2, expected):
    assert mix(s1, s2) == expected
