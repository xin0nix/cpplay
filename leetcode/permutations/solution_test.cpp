#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>
#include <unordered_set>
#include <vector>

using ::testing::UnorderedElementsAre;
using namespace std;

namespace detail {
std::vector<std::string> stringify(const std::vector<std::vector<int>> &input) {
  std::vector<std::string> result;
  result.reserve(input.size());
  for (const auto &inner : input) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < inner.size(); ++i) {
      ss << std::to_string(inner[i]);
      if (i != inner.size() - 1) {
        ss << ",";
      }
    }
    ss << "]";
    result.push_back(ss.str());
  }
  return result;
}
} // namespace detail

struct TopDownSolution {
  struct Impl {
    Impl(const std::vector<int> &nums) : mNums(nums) {}

    [[nodiscard]] vector<vector<int>> getRes() && {
      visit();
      return std::move(mRes);
    }

    void visit(size_t level = 0) {
      if (level == mNums.size()) {
        mRes.push_back(mStack);
        return;
      }
      for (auto n : mNums) {
        if (mVisited.contains(n))
          continue;
        mStack.push_back(n);
        mVisited.insert(n);
        visit(++level);
        --level;
        mVisited.erase(n);
        mStack.pop_back();
      }
    }

  private:
    const std::vector<int> &mNums;
    unordered_set<int> mVisited;
    vector<int> mStack;
    vector<vector<int>> mRes;
  };

  [[nodiscard]] vector<vector<int>> permute(vector<int> &nums) {
    return Impl(nums).getRes();
  }
};

struct BottomUpSolution {
  struct Impl {
    vector<int> &mNums;
    std::vector<vector<int>> visit(size_t level) {
      int curLevelItem = mNums.at(level);
      if (level == mNums.size() - 1) {
        return {{curLevelItem}};
      }
      auto nextLevelPerms = visit(level + 1);
      vector<vector<int>> curLevemPerms;
      for (const auto &subPerm : nextLevelPerms) {
        for (size_t i = 0U; i <= subPerm.size(); ++i) {
          auto perm = subPerm;
          perm.insert(perm.cbegin() + i, curLevelItem);
          curLevemPerms.push_back(std::move(perm));
        }
      }
      return curLevemPerms;
    }
  };
  vector<vector<int>> permute(vector<int> &nums) {
    return Impl{nums}.visit(0U);
  }
};

template <typename T> struct PermutationsTest : ::testing::Test {
  T mSolution;
  T &get() { return mSolution; }
};

struct NameGenerator {
  template <typename T> static std::string GetName(int) {
    if constexpr (std::is_same_v<T, TopDownSolution>)
      return "top-down";
    if constexpr (std::is_same_v<T, BottomUpSolution>)
      return "bottom-up";
  }
};

using Types = ::testing::Types<TopDownSolution, BottomUpSolution>;

TYPED_TEST_SUITE(PermutationsTest, Types, NameGenerator);

TYPED_TEST(PermutationsTest, MetaTest) {
  auto res = ::detail::stringify({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
  EXPECT_THAT(res, UnorderedElementsAre("[1,2,3]", "[4,5,6]", "[7,8,9]"));
}

TYPED_TEST(PermutationsTest, Leet1) {
  vector nums{1, 2, 3};
  auto perms = this->get().permute(nums);
  auto res = ::detail::stringify(perms);
  EXPECT_THAT(res, UnorderedElementsAre("[1,2,3]", "[1,3,2]", "[2,1,3]",
                                        "[2,3,1]", "[3,1,2]", "[3,2,1]"));
}

TYPED_TEST(PermutationsTest, Leet2) {
  vector nums{1, 2};
  auto perms = this->get().permute(nums);
  auto res = ::detail::stringify(perms);
  EXPECT_THAT(res, UnorderedElementsAre("[1,2]", "[2,1]"));
}

TYPED_TEST(PermutationsTest, Leet3) {
  vector nums{1};
  auto perms = this->get().permute(nums);
  auto res = ::detail::stringify(perms);
  EXPECT_THAT(res, UnorderedElementsAre("[1]"));
}
