#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <deque>
#include <functional>
#include <stack>

using namespace std;

class BruteForce {
  stack<int32_t> vals;

  template <typename Op> void exec(Op op) {
    int b = vals.top();
    vals.pop();
    int a = vals.top();
    vals.pop();
    vals.emplace(op(a, b));
  }

  void handleToken(string_view token) {
    if (token == "*")
      return exec(multiplies());
    if (token == "/")
      return exec(divides());
    if (token == "+")
      return exec(plus());
    if (token == "-")
      return exec(minus());
    vals.push(atoi(token.data()));
  }

public:
  int evalRPN(vector<string> &tokens) {
    for (string_view sv : tokens)
      handleToken(sv);
    return vals.top();
  }
};

class EvalPolishNotationTest : public ::testing::Test {
protected:
};

TEST_F(EvalPolishNotationTest, Basic1) {
  vector<string> tokens{"2", "1", "+", "3", "*"};
  ASSERT_EQ(BruteForce().evalRPN(tokens), 9);
}

TEST_F(EvalPolishNotationTest, Basic2) {
  vector<string> tokens{"4", "13", "5", "/", "+"};
  ASSERT_EQ(BruteForce().evalRPN(tokens), 6);
}

TEST_F(EvalPolishNotationTest, Advanced1) {
  vector<string> tokens{"10", "6", "9",  "3", "+", "-11", "*",
                        "/",  "*", "17", "+", "5", "+"};
  ASSERT_EQ(BruteForce().evalRPN(tokens), 22);
}
