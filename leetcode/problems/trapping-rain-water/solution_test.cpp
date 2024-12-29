#include <algorithm>
#include <concepts>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ranges>
#include <vector>

template <typename S>
concept TrappingWaterSolution = requires(S sol, std::vector<int> nums) {
  { sol.trap(nums) } -> std::constructible_from<int>;
};

template <TrappingWaterSolution S>
struct TrappingRainWaterTest : ::testing::Test {
  S &get() & { return _s; }

private:
  S _s;
};

struct BruteForce {
  int trap(const std::vector<int> &height) const noexcept {
    const int len = (int)height.size();
    int sum = 0;
    for (int k = 0; k < len; ++k) {
      int lMax = 0;
      for (int left = k - 1; left >= 0; --left) {
        lMax = std::max(lMax, height[left]);
      }
      int rMax = 0;
      for (int right = k + 1; right < len; ++right) {
        rMax = std::max(rMax, height[right]);
      }
      int potential = std::min(lMax, rMax) - height[k];
      sum += std::max(0, potential);
    }
    return sum;
  }
};

struct RangeBasedLinearMemorySolution {
  int trap(const std::vector<int> &height) const noexcept {
    namespace rng = std::ranges;
    namespace view = std::ranges::views;

    const auto size = height.size();
    // Посчитать максимальные элементы слева от текущего
    std::vector<int> maxLeft{0};
    maxLeft.reserve(size);
    int curMax = 0;
    auto keepMax = [&curMax](const int hVal) {
      curMax = std::max(curMax, hVal);
      return curMax;
    };
    rng::for_each(height | view::take(size - 1) | view::transform(keepMax),
                  [&maxLeft](int val) { maxLeft.push_back(val); });
    // Посчитать максимальные элементы справа от текущего
    std::vector<int> maxRight{0};
    maxRight.reserve(size);
    curMax = 0;
    rng::for_each(height | view::reverse | view::take(size - 1) |
                      view::transform(keepMax),
                  [&maxRight](int val) { maxRight.push_back(val); });

    // // Вычислим перепад по формуле max(0, min(maxL, maxR) - h)
    auto waterLevels = view::zip(maxLeft, maxRight | view::reverse, height) |
                       view::transform([](auto pair) {
                         auto &&[left, right, hVal] = pair;
                         int delta = std::min(left, right) - hVal;
                         return std::max(0, delta);
                       });

    // Сумма всех уровней должна дать финальный результат
    return rng::fold_left(waterLevels, 0, std::plus<int>{});
  }
};

struct TrappingWaterSolutionNameGen {
  template <TrappingWaterSolution S>
  static constexpr std::string GetName(int /* */) {
    if constexpr (std::is_same_v<S, BruteForce>) {
      return "brute-force";
    }
    if constexpr (std::is_same_v<S, RangeBasedLinearMemorySolution>) {
      return "range-linear-memory";
    }
  }
};

using TrappingRainWaterSolutionTypes =
    ::testing::Types<BruteForce, RangeBasedLinearMemorySolution>;
TYPED_TEST_SUITE(TrappingRainWaterTest, TrappingRainWaterSolutionTypes,
                 TrappingWaterSolutionNameGen);

TYPED_TEST(TrappingRainWaterTest, Leet1) {
  std::vector height{0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
  auto res = this->get().trap(height);
  EXPECT_EQ(res, 6);
  std::reverse(height.begin(), height.end());
  res = this->get().trap(height);
  EXPECT_EQ(res, 6);
}

TYPED_TEST(TrappingRainWaterTest, Leet2) {
  std::vector height{4, 2, 0, 3, 2, 5};
  auto res = this->get().trap(height);
  EXPECT_EQ(res, 9);
  std::reverse(height.begin(), height.end());
  res = this->get().trap(height);
  EXPECT_EQ(res, 9);
}

TYPED_TEST(TrappingRainWaterTest, Ladder) {
  std::vector height{0, 1, 2, 3};
  auto res = this->get().trap(height);
  EXPECT_EQ(res, 0);
  std::reverse(height.begin(), height.end());
  res = this->get().trap(height);
  EXPECT_EQ(res, 0);
}

TYPED_TEST(TrappingRainWaterTest, Empty) {
  std::vector height{0, 0, 0, 0};
  auto res = this->get().trap(height);
  EXPECT_EQ(res, 0);
}

TYPED_TEST(TrappingRainWaterTest, Full) {
  std::vector height{100, 100, 100, 100};
  auto res = this->get().trap(height);
  EXPECT_EQ(res, 0);
}

TYPED_TEST(TrappingRainWaterTest, MiddlePole) {
  std::vector height{0, 0, 42, 0, 0};
  auto res = this->get().trap(height);
  EXPECT_EQ(res, 0);
}

TYPED_TEST(TrappingRainWaterTest, ComplexA) {
  std::vector height{0, 2, 0, 4, 1, 0, 3, 6, 2, 3, 4, 3};
  auto res = this->get().trap(height);
  EXPECT_EQ(res, 13);
  std::reverse(height.begin(), height.end());
  res = this->get().trap(height);
  EXPECT_EQ(res, 13);
}

TYPED_TEST(TrappingRainWaterTest, ComplexB) {
  std::vector height{5, 3, 2, 0, 1, 6, 0};
  auto res = this->get().trap(height);
  EXPECT_EQ(res, 14);
  std::reverse(height.begin(), height.end());
  res = this->get().trap(height);
  EXPECT_EQ(res, 14);
}