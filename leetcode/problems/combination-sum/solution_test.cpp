#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <deque>
#include <span>
#include <vector>

using std::vector;
using ::testing::UnorderedElementsAre;

struct BruteForce {
  struct StackGuard {
    std::reference_wrapper<std::deque<int>> mCurrentCombo;
    std::reference_wrapper<int> mCurrentSum;
    const int mElement;
    explicit StackGuard(std::reference_wrapper<std::deque<int>> currentCombo,
                        std::reference_wrapper<int> currentSumm,
                        const int element) noexcept
        : mCurrentCombo(currentCombo), mCurrentSum(currentSumm),
          mElement(element) {
      mCurrentCombo.get().push_back(element);
      mCurrentSum.get() += element;
    }
    ~StackGuard() noexcept {
      mCurrentCombo.get().pop_back();
      mCurrentSum.get() -= mElement;
    }
  };

  struct Implementation {
    const int mTarget;
    int mCurrentSum = 0;
    vector<vector<int>> mCombinations;
    std::deque<int> mCurrentCombo;

    explicit Implementation(const int target) : mTarget(target) {}

    Implementation &visit(std::span<int> candidates) {
      if (!candidates.empty()) {
        visit(candidates.front(), candidates.subspan(1));
      }
      return *this;
    }

    void visit(const int element, std::span<int> rest) {
      // Early exit - the sum is greater than the target
      if (mCurrentSum > mTarget) {
        return;
      }
      // First option - skip the element and go on with the rest
      if (!rest.empty()) {
        visit(rest.front(), rest.subspan(1));
      }
      // Second option - continue adding more elements while keeping rest the
      // same
      StackGuard stackGuard{std::ref(mCurrentCombo), std::ref(mCurrentSum),
                            element};
      // We found a new combination, save it
      if (mCurrentSum == mTarget) {
        std::vector<int> newCombo;
        newCombo.reserve(mCurrentCombo.size());
        std::copy(mCurrentCombo.cbegin(), mCurrentCombo.cend(),
                  std::back_inserter(newCombo));
        mCombinations.push_back(std::move(newCombo));
        // It's pointless to continue the seach as for any new item we will
        // end up with a sum greater than the target
        return;
      }
      return visit(element, rest);
    }

    std::vector<std::vector<int>> &&consumeCombinations() {
      return std::move(mCombinations);
    }
  };
  vector<vector<int>> combinationSum(vector<int> &candidates, int target) {
    return Implementation(target).visit(candidates).consumeCombinations();
  }
};

TEST(SolutionTest, LeetCase1) {
  vector candidates{2, 3, 6, 7};
  int target = 7;
  BruteForce s;
  auto combinations = s.combinationSum(candidates, target);
  EXPECT_EQ(combinations.size(), 2);
  EXPECT_THAT(combinations[0], UnorderedElementsAre(7));
  EXPECT_THAT(combinations[1], UnorderedElementsAre(2, 2, 3));
}

TEST(SolutionTest, LeetCase2) {
  vector candidates{2, 3, 5};
  int target = 8;
  BruteForce s;
  auto combinations = s.combinationSum(candidates, target);
  EXPECT_EQ(combinations.size(), 3);
  EXPECT_THAT(combinations[0], UnorderedElementsAre(3, 5));
  EXPECT_THAT(combinations[1], UnorderedElementsAre(2, 3, 3));
  EXPECT_THAT(combinations[2], UnorderedElementsAre(2, 2, 2, 2));
}
