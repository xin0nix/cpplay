#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <span>
#include <vector>

using std::vector;
using ::testing::ElementsAreArray;
using ::testing::UnorderedElementsAreArray;

#include <ranges>
namespace rng = std::ranges;
namespace view = std::views;
/**
 * @see: https://leetcode.com/problems/combination-sum-ii/
 */
struct Solution {

  vector<vector<int>> combinationSum2(vector<int> &candidates, int target) {
    rng::sort(candidates);
    struct {
      auto operator()(std::span<int> vals, int target) {
        if (vals.empty()) {
          return;
        }

        int front = vals.front();
        if (front == target) {
          result.emplace_back(stack).push_back(front);
          return;
        }

        if (front > target) {
          return;
        }

        // Вариант 1: взять front
        {
          stack.push_back(front);
          operator()({vals.begin() + 1, vals.end()}, target - front);
          stack.pop_back();
        }

        // Вариант 2: пропустить все элементы, равные front
        auto nextRange = vals | std::views::drop_while(
                                    [front](int val) { return val == front; });
        operator()({nextRange.begin(), nextRange.end()}, target);
      }
      vector<vector<int>> result;
      vector<int> stack;
    } impl{};
    impl(candidates, target);
    return impl.result;
  }
};

// Helper matcher for unordered vector of unique combinations
// For example, [1, 2] and [2, 1] is the same combination,
// while [3] and [1, 2] are distinct (and unique o
auto ElementsAreCombinations(auto &&result, auto &&expected) {
  auto stringify = [](auto &vals) {
    std::stringstream svals;
    svals << "|";
    for (auto val : vals) {
      svals << val << "|";
    }
    return svals.str();
  };
  rng::sort(result);
  rng::sort(expected);
  auto resultStrs = rng::views::transform(result, stringify) |
                    std::ranges::to<std::vector<std::string>>();
  auto expectedStrs = rng::views::transform(expected, stringify) |
                      std::ranges::to<std::vector<std::string>>();
  EXPECT_THAT(resultStrs, UnorderedElementsAreArray(expectedStrs));
}

struct TestCase {
  std::string name;
  std::vector<int> candidates;
  int target;
  std::vector<std::vector<int>> expected;
};

std::ostream &operator<<(std::ostream &outs, const TestCase &tcase) {
  // Lambda to print a vector<int> in [x, y, z] format
  auto printVector = [&outs](const std::vector<int> &vec) {
    outs << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
      outs << vec[i];
      if (i + 1 < vec.size()) {
        outs << ", ";
      }
    }
    outs << "]";
  };

  outs << "TestCase: " << tcase.name << "\n";

  outs << "  Candidates: ";
  printVector(tcase.candidates);
  outs << "\n";

  outs << "  Target: " << tcase.target << "\n";

  outs << "  Expected combinations:\n";
  if (tcase.expected.empty()) {
    outs << "    (none)\n";
  } else {
    for (const auto &combo : tcase.expected) {
      outs << "    ";
      printVector(combo);
      outs << "\n";
    }
  }
  return outs;
}

class CombinationSum2Test : public ::testing::TestWithParam<TestCase> {};

TEST_P(CombinationSum2Test, HandlesAllCases) {
  const auto &param = GetParam();

  SCOPED_TRACE(param.name);

  Solution sol;
  std::vector cans = param.candidates;
  std::vector exp = param.expected;
  auto result = sol.combinationSum2(cans, param.target);

  if (exp.empty()) {
    EXPECT_THAT(result, ::testing::IsEmpty());
  } else {
    ElementsAreCombinations(std::move(result), std::move(exp));
  }
}

INSTANTIATE_TEST_SUITE_P(
    CombinationSum2All, CombinationSum2Test,
    ::testing::Values(
        TestCase{"Test Case 1: Basic example from problem statement",
                 {10, 1, 2, 7, 6, 1, 5},
                 8,
                 {{1, 1, 6}, {1, 2, 5}, {1, 7}, {2, 6}}},
        TestCase{"Test Case 2: Duplicates with single-element combination",
                 {2, 5, 2, 1, 2},
                 5,
                 {{1, 2, 2}, {5}}},
        TestCase{"Test Case 3: Single element matching target", {5}, 5, {{5}}},
        TestCase{"Test Case 4: No valid combinations", {3}, 1, {}},
        TestCase{"Test Case 5: All elements same with valid combination",
                 {2, 2, 2, 2, 2},
                 4,
                 {{2, 2}}},
        TestCase{"Test Case 6: Skip duplicates after first usage",
                 {1, 1, 2},
                 3,
                 {{1, 2}}},
        TestCase{"Test Case 7: Multiple valid combinations",
                 {1, 2, 3, 4, 2},
                 4,
                 {{1, 3}, {4}, {2, 2}}},
        TestCase{"Test Case 8: All candidates exceed target", {3, 5, 7}, 2, {}},
        TestCase{"Test Case 9: Early termination due to sorted candidates",
                 {1, 2, 2, 2},
                 5,
                 {{1, 2, 2}}},
        TestCase{"Test Case 10: All elements used in combination",
                 {1, 1, 1},
                 3,
                 {{1, 1, 1}}},
        TestCase{"Test Case 11: Multiple duplicates with target 4",
                 {1, 1, 1, 1, 1, 2, 2, 2},
                 4,
                 {{1, 1, 2}, {2, 2}, {1, 1, 1, 1}}}));
