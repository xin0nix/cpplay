#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <deque>
#include <iostream>
#include <stack>

using namespace std;

class MinStack {
  stack<pair<int, int>> vals;

public:
  MinStack() {}

  void push(int val) {
    if (vals.empty()) [[unlikely]]
      vals.push({val, val});
    else
      vals.push({val, min(getMin(), val)});
  }

  void pop() { vals.pop(); }

  int top() { return vals.top().first; }

  int getMin() { return vals.top().second; }
};

class MinStackTest : public ::testing::Test {};

TEST_F(MinStackTest, Basic) {
  MinStack s;
  s.push(-2);
  s.push(0);
  s.push(-3);
  EXPECT_EQ(s.getMin(), -3);
  s.pop();
  EXPECT_EQ(s.top(), 0);
  EXPECT_EQ(s.getMin(), -2);
}
