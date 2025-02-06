#include <gtest/gtest.h>

#include <ranges>
#include <string>
namespace rng = std::ranges;
namespace view = rng::views;

struct Solution {
  template <typename R> auto enumerate(R &&digits) {
    return view::zip(view::iota(0), std::forward<R>(digits));
  };
  int numDecodings(std::string nums) {
    if (nums.empty()) {
      return 0;
    }
    auto digits = nums | view::transform([](char letter) -> int {
                    return int(letter - '0');
                  });
    int top = 0;
    int prevTop = 0;
    int higher = -1;
    int lower = -1;
    auto helper = [&]() {
      // Если цифра в старшем регистре равна нулю - вариантов нет
      if (higher == 0) {
        return 0;
      }
      // Мы в конце последовательности - добавляем единицу
      // >0 -1 || -1 -1
      if (higher == -1 || lower == -1) {
        return 1;
      }
      // Цифра в старшем разряде точно может быть декодирована
      // Результа зависит от того, образуется ли корректное число
      // >0 ?
      int res = prevTop;
      if (int sum = (10 * higher) + lower; sum <= 26) {
        return top + prevTop;
      }
      return top;
    };
    for (auto digit : digits | view::reverse) {
      auto cur = helper();
      lower = higher;
      higher = digit;
      prevTop = top;
      top = cur;
    }
    return helper();
  }
};

TEST(NumDecodingsTest, Leet) {
  Solution sol{};
  {
    SCOPED_TRACE("Case 1");
    EXPECT_EQ(sol.numDecodings("12"), 2);
  }
  {
    SCOPED_TRACE("Case 2");
    EXPECT_EQ(sol.numDecodings("226"), 3);
  }
  {
    SCOPED_TRACE("Case 3");
    EXPECT_EQ(sol.numDecodings("06"), 0);
  }
}

TEST(NumDecodingsTest, Custom) {
  Solution sol{};
  {
    SCOPED_TRACE("Case 1");
    EXPECT_EQ(sol.numDecodings(""), 0);
  }
  {
    SCOPED_TRACE("Case 2");
    EXPECT_EQ(sol.numDecodings("0"), 0);
  }
  {
    SCOPED_TRACE("Case 3");
    EXPECT_EQ(sol.numDecodings("00"), 0);
  }
  {
    SCOPED_TRACE("Case 4");
    EXPECT_EQ(sol.numDecodings("000"), 0);
  }
}