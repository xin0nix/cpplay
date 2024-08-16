#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>
#include <unordered_set>
#include <vector>

using ::testing::UnorderedElementsAre;
using namespace std;

namespace {
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
} // namespace

struct Solution {
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

TEST(Permutations, MetaTest) {
  auto res = ::stringify({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
  EXPECT_THAT(res, UnorderedElementsAre("[1,2,3]", "[4,5,6]", "[7,8,9]"));
}

TEST(Permutations, Leet1) {
  vector nums{1, 2, 3};
  auto perms = Solution().permute(nums);
  auto res = stringify(perms);
  EXPECT_THAT(res, UnorderedElementsAre("[1,2,3]", "[1,3,2]", "[2,1,3]",
                                        "[2,3,1]", "[3,1,2]", "[3,2,1]"));
}
