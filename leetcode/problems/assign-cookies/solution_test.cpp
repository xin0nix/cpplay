#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

#include <ranges>
namespace rng = std::ranges;
namespace view = rng::views;

struct Solution {
  int findContentChildren(std::vector<int> &greed, std::vector<int> &cookies) {
    rng::sort(greed);
    rng::sort(cookies);
    // kid будет хранить ответ, т.к. мы увеличиваем его только когда ребёнок
    // съедает печенье (менее жадные дети выигрывают, даже если мы могли
    // накормить тем же печеньем более жадных)
    auto kid = 0UL;
    auto idx = 0UL;
    const auto numKids = greed.size();
    const auto numCookies = cookies.size();

    while (kid < numKids and idx < numCookies) {
      // Если ребёнок слишком жадный - ищём следующее печенье
      // - все дети до него были менее жадными и уже получили печенье
      // - все дети после него будут либо такими же, либо ещё более жадными
      if (greed.at(kid) > cookies.at(idx)) {
        // Печенье больше никто не съест, сдвигаем idx вправо
        idx += 1;
        continue;
      }

      // Ребёнок съел печенье, сдвигаем оба индекса вправо
      kid += 1;
      idx += 1;
    }
    return (int)kid;
  }
};

class ContentChildrenTest : public ::testing::Test {
protected:
  Solution solution;
};

TEST_F(ContentChildrenTest, Test1) {
  std::vector g = {1, 2, 3};
  std::vector s = {1, 1};
  EXPECT_EQ(1, solution.findContentChildren(g, s));
}

TEST_F(ContentChildrenTest, Test1Opposite) {
  std::vector g = {1, 1};
  std::vector s = {1, 2, 3};
  EXPECT_EQ(2, solution.findContentChildren(g, s));
}

TEST_F(ContentChildrenTest, Test2) {
  std::vector g = {1, 2};
  std::vector s = {1, 2, 3};
  EXPECT_EQ(2, solution.findContentChildren(g, s));
}

TEST_F(ContentChildrenTest, Test2Opposite) {
  std::vector g = {1, 2, 3};
  std::vector s = {1, 2};
  EXPECT_EQ(2, solution.findContentChildren(g, s));
}

TEST_F(ContentChildrenTest, Test3) {
  std::vector g = {101, 102, 103};
  std::vector s = {1, 2, 3, 4, 5, 6, 6};
  EXPECT_EQ(0, solution.findContentChildren(g, s));
}

TEST_F(ContentChildrenTest, Test3Opposite) {
  std::vector g = {1, 2, 3, 4, 5, 6, 6};
  std::vector s = {101, 102, 103};
  EXPECT_EQ(3, solution.findContentChildren(g, s));
}

TEST_F(ContentChildrenTest, Test4) {
  std::vector g = {101, 102, 103, 105, 106};
  std::vector s = {1, 2, 3};
  EXPECT_EQ(0, solution.findContentChildren(g, s));
}

TEST_F(ContentChildrenTest, Test4Opposite) {
  std::vector g = {1, 2, 3, 4};
  std::vector s = {101, 102, 103, 105, 105, 106};
  EXPECT_EQ(4, solution.findContentChildren(g, s));
}

TEST_F(ContentChildrenTest, Test5) {
  std::vector g = {3, 3, 3, 3, 3};
  std::vector s = {4, 4, 4};
  EXPECT_EQ(3, solution.findContentChildren(g, s));
}

TEST_F(ContentChildrenTest, Test5Opposite) {
  std::vector g = {4, 4, 4};
  std::vector s = {3, 3, 3, 3, 3};
  EXPECT_EQ(0, solution.findContentChildren(g, s));
}
