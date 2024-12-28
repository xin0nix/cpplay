#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

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

using std::vector;

struct BruteForce {
  vector<vector<int>> subsetsWithDup(vector<int> &nums) {
    std::sort(nums.begin(), nums.end());
    struct {
      void visit(size_t i = 0) {
        if (i == mNums.size()) {
          mSubsets.push_back(mStack);
          return;
        }
        int cur = mNums[i];
        mStack.push_back(cur);
        i += 1;
        visit(i);
        mStack.pop_back();
        size_t j = i;
        for (; j < mNums.size(); ++j) {
          if (cur != mNums[j]) {
            break;
          }
        }
        visit(j);
      }
      vector<vector<int>> getRes() && { return std::move(mSubsets); }
      const vector<int> &mNums;
      vector<int> mStack;
      vector<vector<int>> mSubsets;
    } impl{.mNums = nums};
    impl.visit();
    return std::move(impl).getRes();
  }
};

TEST(Subsets2Test, Leet) {
  {
    SCOPED_TRACE("Case 1");
    vector nums{2, 1, 2};
    EXPECT_THAT(detail::stringify(BruteForce().subsetsWithDup(nums)),
                ::testing::UnorderedElementsAre("[]", "[1]", "[1,2]", "[1,2,2]",
                                                "[2]", "[2,2]"));
  }
  {
    SCOPED_TRACE("Case 2");
    vector nums{1};
    EXPECT_THAT(detail::stringify(BruteForce().subsetsWithDup(nums)),
                ::testing::UnorderedElementsAre("[]", "[1]"));
  }
}

TEST(Subsets2Test, Neet) {
  vector nums{3, 1, 2, 2};
  EXPECT_THAT(detail::stringify(BruteForce().subsetsWithDup(nums)),
              ::testing::UnorderedElementsAre(
                  "[1,2,2,3]", "[1,2,2]", "[1,2]", "[1,2,3]", "[1,3]", "[1]",
                  "[2,2,3]", "[2,2]", "[2,3]", "[2]", "[3]", "[]"));
}
