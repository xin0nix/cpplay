#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdint>
#include <deque>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <vector>

using ::testing::UnorderedElementsAreArray;

#include <ranges>
namespace rng = std::ranges;
namespace view = std::views;
struct Solution {
  using Digit = std::uint32_t;
  using Number = std::uint32_t;
  using SplitRes = std::deque<std::array<Number, 4UL>>;
  constexpr std::optional<Number> getNum(std::span<Digit> digits) noexcept {
    if (digits.empty() or digits.size() > 3UL) {
      return std::nullopt;
    }
    if (digits.front() == 0 and digits.size() > 1) {
      return std::nullopt;
    }
    int power = -1;
    int num = rng::fold_left(digits | view::reverse, 0,
                             [&power](int left, int right) -> int {
                               power += 1;
                               switch (power) {
                               case 0:
                                 return left + right;
                               case 1:
                                 return left + (right * 10);
                               case 2:
                                 return left + (right * 100);
                               default:
                                 return -1;
                               }
                             });
    if (num < 0 or num > 255) {
      return std::nullopt;
    }
    return num;
  }

  SplitRes split(std::span<Digit> digits, std::uint32_t level = 0) noexcept {
    if (digits.empty()) {
      if (level == 4UL) {
        return SplitRes{{}};
      }
      return {};
    }
    if (level > 4UL) {
      return {};
    }
    std::size_t start = 1UL;
    std::size_t stop = std::min(digits.size(), 3UL);
    SplitRes res{};
    for (auto idx : view::iota(start) | view::take(stop)) {
      auto head = getNum(digits | view::take(idx));
      if (!head) {
        continue;
      }
      auto tail = split(digits | view::drop(idx), level + 1);
      if (tail.empty()) {
        continue;
      }
      for (auto &&item : tail) {
        item.at(level) = *head;
        res.push_back(item);
      }
    }
    return res;
  }

  std::vector<std::string> restoreIpAddresses(const std::string &str) noexcept {
    if (str.size() < 4 or str.size() > 12) {
      return {};
    }
    auto digitized = str | view::transform([](char letter) -> Digit {
                       return Digit(letter) - Digit('0');
                     });
    auto nums = rng::to<std::vector<Digit>>(digitized);
    auto ips = split(nums);
    auto ipStrings = ips | view::transform([](auto &&ipd) -> std::string {
                       std::stringstream sss{};
                       sss << ipd[0] << '.' << ipd[1] << '.' << ipd[2] << '.'
                           << ipd[3];
                       return sss.str();
                     });
    return rng::to<std::vector<std::string>>(ipStrings);
  }
};

TEST(SolutionTest, TestLeet1) {
  std::string str = "25525511135";
  std::vector<std::string> expected = {"255.255.11.135", "255.255.111.35"};
  std::vector<std::string> ips = Solution().restoreIpAddresses(str);
  EXPECT_THAT(ips, testing::UnorderedElementsAreArray(expected));
}

TEST(SolutionTest, TestLeet2) {
  std::string str = "0000";
  std::vector<std::string> expected = {"0.0.0.0"};
  std::vector<std::string> ips = Solution().restoreIpAddresses(str);
  EXPECT_THAT(ips, testing::UnorderedElementsAreArray(expected));
}

TEST(SolutionTest, TestLeet3) {
  std::string str = "102045";
  std::vector<std::string> expected = {"1.0.20.45", "1.0.204.5", "10.2.0.45",
                                       "10.20.4.5", "102.0.4.5"};
  std::vector<std::string> ips = Solution().restoreIpAddresses(str);
  EXPECT_THAT(ips, testing::UnorderedElementsAreArray(expected));
}

TEST(SolutionTest, TestLeegBug1) {
  std::string str = "010010";
  std::vector<std::string> expected = {"0.10.0.10", "0.100.1.0"};
  std::vector<std::string> ips = Solution().restoreIpAddresses(str);
  EXPECT_THAT(ips, testing::UnorderedElementsAreArray(expected));
}

TEST(SolutionTest, TestBad1) {
  std::vector<std::string> expected = {};
  std::vector<std::string> ips = Solution().restoreIpAddresses("");
  EXPECT_THAT(ips, testing::UnorderedElementsAreArray(expected));
}

TEST(SolutionTest, TestBad2) {
  std::string str = "42";
  std::vector<std::string> expected = {};
  std::vector<std::string> ips = Solution().restoreIpAddresses(str);
  EXPECT_THAT(ips, testing::UnorderedElementsAreArray(expected));
}

TEST(SolutionTest, TestCorner0) {
  std::string str = "1234";
  std::vector<std::string> expected = {"1.2.3.4"};
  std::vector<std::string> ips = Solution().restoreIpAddresses(str);
  EXPECT_THAT(ips, testing::UnorderedElementsAreArray(expected));
}

TEST(SolutionTest, TestCorner1) {
  std::string str = "100100100100";
  std::vector<std::string> expected = {"100.100.100.100"};
  std::vector<std::string> ips = Solution().restoreIpAddresses(str);
  EXPECT_THAT(ips, testing::UnorderedElementsAreArray(expected));
}

TEST(SolutionTest, TestCorner2) {
  std::string str = "255255255255";
  std::vector<std::string> expected = {"255.255.255.255"};
  std::vector<std::string> ips = Solution().restoreIpAddresses(str);
  EXPECT_THAT(ips, testing::UnorderedElementsAreArray(expected));
}

TEST(SolutionTest, GetNumTests) {
  Solution sol;
  std::vector<Solution::Digit> nums;
  EXPECT_EQ(sol.getNum(nums = {0}), 0);
  EXPECT_EQ(sol.getNum(nums = {0, 0}), std::nullopt);
  EXPECT_EQ(sol.getNum(nums = {2, 5}), 25);
  EXPECT_EQ(sol.getNum(nums = {2, 5, 5}), 255);
  EXPECT_EQ(sol.getNum(nums = {2, 5, 5, 5}), std::nullopt);
}