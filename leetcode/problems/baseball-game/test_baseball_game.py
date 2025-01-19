class Solution:
    def calPoints(self, operations: list[str]) -> int:
        # Начинаем с пустого стека
        stack = []
        # Теперь просто выполняем команды как в инструкции
        for op in operations:
            if op == "+":
                [a, b] = stack[-2:]
                stack.append(a + b)
            elif op == "C":
                stack.pop()
            elif op == "D":
                a = stack[-1]
                stack.append(2 * a)
            else:
                x = int(op)
                stack.append(x)
        return sum(stack)


def test_calPoints():
    assert Solution().calPoints(["5", "2", "C", "D", "+"]) == 30
    assert Solution().calPoints(["5", "-2", "4", "C", "D", "9", "+", "+"]) == 27
    assert Solution().calPoints(["1", "C"]) == 0
