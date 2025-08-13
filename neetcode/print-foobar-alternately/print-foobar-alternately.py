from enum import Enum
import threading

from typing import Callable


def printFoo():
    print("foo", end="")


def printBar():
    print("bar", end="")


class State(Enum):
    FOO = 1
    BAR = 2


class FooBar:
    def __init__(self, n):
        self.n = n
        self.condition = threading.Condition()
        self.state = State(1)

    def foo(self, printFoo: "Callable[[], None]") -> None:
        for _ in range(self.n):
            with self.condition:
                while self.state != State.FOO:
                    self.condition.wait()
                printFoo()
                self.state = State.BAR
                self.condition.notify()

    def bar(self, printBar: "Callable[[], None]") -> None:
        for _ in range(self.n):
            with self.condition:
                while self.state != State.BAR:
                    self.condition.wait()
                printBar()
                self.state = State.FOO
                self.condition.notify()


obj = FooBar(10)


def worker1():
    obj.foo(printFoo=printFoo)


def worker2():
    obj.bar(printBar=printBar)


if __name__ == "__main__":
    t1 = threading.Thread(target=worker1)
    t2 = threading.Thread(target=worker2)
    t1.start()
    t2.start()
    t1.join()
    t2.join()
