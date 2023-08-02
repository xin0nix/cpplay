#include "solution.hpp"

#include <algorithm>
#include <deque>
#include <unordered_map>

std::vector<int> BasicSolution::topKFrequent(std::vector<int> &nums, int k) {
  // First - calculate the frequencies
  std::unordered_map<int, int> freq;
  for (int n : nums)
    freq[n]++;
  // Second - find the most frequent values
  std::vector<std::pair<int, int>> qerf;
  qerf.reserve(freq.size());
  for (auto &p : freq)
    qerf.push_back(std::move(p));
  std::sort(qerf.begin(), qerf.end(),
            [](auto &a, auto &b) { return a.second > b.second; });
  std::vector<int> answer;
  answer.reserve(k);
  for (int i = 0; i < k; ++i)
    answer.push_back(qerf[i].first);
  return answer;
}

std::vector<int> Solution::topKFrequent(std::vector<int> &nums, int k) {
  return BasicSolution().topKFrequent(nums, k);
}
