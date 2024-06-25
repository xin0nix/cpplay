#pragma once

#include <string>
#include <vector>

// Problem statement: https://leetcode.com/problems/group-anagrams/

struct SolutionImpl {
  virtual std::vector<std::vector<std::string>>
  groupAnagrams(const std::vector<std::string> &strs) = 0;
};

// Leetcode score:
// Runtime: 28 ms Beats 95.86 %
// Memory: 18.80mb Beats 91.86 %
struct MapSolution : SolutionImpl {
  virtual std::vector<std::vector<std::string>>
  groupAnagrams(const std::vector<std::string> &strs) override final;
};

struct Solution {
  std::vector<std::vector<std::string>>
  groupAnagrams(std::vector<std::string> &strs);
};
