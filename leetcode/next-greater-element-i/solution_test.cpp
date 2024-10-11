#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iterator>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using ::testing::ElementsAre;

namespace {
template <typename Iterable> struct enumerate_object {
  Iterable &iterable;
  using iterator = decltype(std::begin(std::declval<Iterable>()));

  struct Iterator {
    iterator iter;
    std::size_t index;

    Iterator(iterator it, std::size_t idx) : iter(it), index(idx) {}
    auto operator*() const { return std::make_tuple(index, *iter); }
    Iterator &operator++() {
      ++iter;
      ++index;
      return *this;
    }
    bool operator!=(const Iterator &other) const { return iter != other.iter; }
  };

  enumerate_object(Iterable &it) : iterable(it) {}
  auto begin() { return Iterator(std::begin(iterable), 0); }
  auto end() { return Iterator(std::end(iterable), 0); }
};

template <typename Iterable> auto enumerate(Iterable &&iterable) {
  return enumerate_object<Iterable>(iterable);
}
} // namespace

struct Solution {
  // Function to be tested
  std::vector<int> nextGreaterElement(std::vector<int> &nums1,
                                      std::vector<int> &nums2) {
    std::vector<int> result;
    result.reserve(nums1.size());
    std::unordered_map<int, size_t> val2index;
    // Store indicies of nums2 in the cache
    for (auto [index, value] : enumerate(nums2)) {
      val2index[value] = index;
    }

    // Given nums1[i], find nums2[j] such that nums2[j] == nums1[i]
    for (const auto n : nums1) {
      auto it = nums2.cbegin();
      std::advance(it, val2index[n]);
      // Look up for nums2[k], such that k > j, nums2[k] > nums2[j]
      auto next = std::find_if(it, nums2.cend(), [n](auto x) { return x > n; });
      if (next != nums2.cend()) {
        result.push_back(*next);
        continue;
      }
      // Otherwise, use -1
      result.push_back(-1);
    }
    return result;
  }
};

TEST(NextGreaterElementTest, BasicTest) {
  std::vector<int> nums1 = {4, 1, 2};
  std::vector<int> nums2 = {1, 3, 4, 2};
  EXPECT_THAT(Solution().nextGreaterElement(nums1, nums2),
              ElementsAre(-1, 3, -1));
}

TEST(NextGreaterElementTest, EmptyNums1Test) {
  std::vector<int> nums1 = {};
  std::vector<int> nums2 = {1, 2, 3};
  std::vector<int> expected = {};
  std::vector<int> actual = Solution().nextGreaterElement(nums1, nums2);
  EXPECT_EQ(expected, actual);
}

TEST(NextGreaterElementTest, ExampleTest1) {
  std::vector<int> nums1 = {1, 3, 2};
  std::vector<int> nums2 = {4, 2, 3, 0, 1, 6, 5};
  EXPECT_THAT(Solution().nextGreaterElement(nums1, nums2),
              ElementsAre(6, 6, 3));
}

TEST(NextGreaterElementTest, AnotherExampleTest) {
  std::vector<int> nums1 = {1, 3, 2, 6, 5, 0};
  std::vector<int> nums2 = {3, 0, 4, 1, 2, 5, 6};
  EXPECT_THAT(Solution().nextGreaterElement(nums1, nums2),
              ElementsAre(2, 4, 5, -1, 6, 4));
}