import threading
from typing import Callable
from enum import Enum


class State(Enum):
    ZERO_ODD = 0
    ODD = 1
    ZERO_EVEN = 2
    EVEN = 3


class ZeroEvenOdd:

    def __init__(self, n):
        self.n = n
        self.condition = threading.Condition()
        self.state = State.ZERO_ODD
        self.done = False
        self.current = 0

    def zero(self, printNumber: "Callable[[int], None]") -> None:
        while True:
            with self.condition:
                while (
                    not self.done
                    and self.state != State.ZERO_ODD
                    and self.state != State.ZERO_EVEN
                ):
                    self.condition.wait()
                if self.done is True:
                    return
                printNumber(0)
                if self.state == State.ZERO_ODD:
                    self.state = State.ODD
                else:
                    assert self.state == State.ZERO_EVEN
                    self.state = State.EVEN
                self.condition.notify_all()

    def handle_number(self, printNumber, state_from: State, state_into: State) -> None:
        while True:
            with self.condition:
                while not self.done and self.state != state_from:
                    self.condition.wait()
                if self.done is True:
                    return
                self.current += 1
                printNumber(self.current)
                if self.current == self.n:
                    self.done = True
                self.state = state_into
                self.condition.notify_all()

    def odd(self, printNumber: "Callable[[int], None]") -> None:
        self.handle_number(printNumber, State.ODD, State.ZERO_EVEN)

    def even(self, printNumber: "Callable[[int], None]") -> None:
        self.handle_number(printNumber, State.EVEN, State.ZERO_ODD)


NUMBER = 2

obj = ZeroEvenOdd(NUMBER)


def printNumber(number):
    print(number, end="")


def worker1():
    obj.zero(printNumber=printNumber)


def worker2():
    obj.even(printNumber=printNumber)


def worker3():
    obj.odd(printNumber=printNumber)


if __name__ == "__main__":
    t1 = threading.Thread(target=worker1)
    t2 = threading.Thread(target=worker2)
    t3 = threading.Thread(target=worker3)
    t1.start()
    t2.start()
    t3.start()
    t1.join()
    t2.join()
    t3.join()
