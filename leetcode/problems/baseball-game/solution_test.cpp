#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <deque>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <variant>

namespace rng = std::ranges;
namespace view = std::views;

// helper type for the visitor
template <class... Ts> struct overloads : Ts... {
  using Ts::operator()...;
};

struct Solution {
  struct Value {
    int x{0};
  };
  struct AddOp {};
  struct ClearOp {};
  struct DuplicateOp {};

  using OpType = std::variant<Value, AddOp, ClearOp, DuplicateOp>;

  static int calPoints(const std::span<std::string> rawOps) noexcept {
    auto parsedOps =
        rawOps | view::transform([](const auto &operation) -> OpType {
          if (operation == "+") {
            return AddOp{};
          }
          if (operation == "C") {
            return ClearOp{};
          }
          if (operation == "D") {
            return DuplicateOp{};
          }
          Value val;
          std::istringstream(operation) >> val.x;
          return val;
        });
    std::deque<int> stack;
    rng::for_each(parsedOps, [&stack](OpType operation) {
      std::visit(overloads{
                     [&stack](Value val) { stack.push_back(val.x); },
                     [&stack](AddOp) {
                       auto lastTwo = stack | view::reverse | view::take(2);
                       int sum = rng::fold_left(lastTwo, 0, std::plus<int>{});
                       stack.push_back(sum);
                     },
                     [&stack](ClearOp) { stack.pop_back(); },
                     [&stack](DuplicateOp) {
                       int val = stack.back();
                       stack.push_back(2 * val);
                     },
                 },
                 operation);
    });
    return rng::fold_left(stack, 0, std::plus<int>{});
  }
};

TEST(BaseBallGameTest, Leet1) {
  std::array<std::string, 5> ops{"5", "2", "C", "D", "+"};
  auto score = Solution::calPoints(ops);
  EXPECT_EQ(score, 30);
}

TEST(BaseBallGameTest, Leet2) {
  std::array<std::string, 8> ops{"5", "-2", "4", "C", "D", "9", "+", "+"};
  auto score = Solution::calPoints(ops);
  EXPECT_EQ(score, 27);
}

TEST(BaseBallGameTest, Leet3) {
  std::array<std::string, 2> ops{"1", "C"};
  auto score = Solution::calPoints(ops);
  EXPECT_EQ(score, 0);
}
