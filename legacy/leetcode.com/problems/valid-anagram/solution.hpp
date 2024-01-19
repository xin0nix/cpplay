#pragma once

#include <string>

// Problem statement: https://leetcode.com/problems/valid-anagram/

struct SolutionImpl {
  virtual bool isAnagram(std::string s, std::string t) = 0;
};

// Runtime 16ms Beats 41.93 %
// Memory 7.28mb Beats 79.57 %
struct SortSolution : SolutionImpl {
  virtual bool isAnagram(std::string s, std::string t) override final;
};

// Runtime 13ms Beats 54.83 %
// Memory 7.31mb Beats 39.24 %
struct MapSolution : SolutionImpl {
  virtual bool isAnagram(std::string s, std::string t) override final;
};

// Runtime 3ms Beats 98.37 %
// Memory 7.09 mb Beats 99.69%
struct ArraySolution : SolutionImpl {
  int freq[26];
  virtual bool isAnagram(std::string s, std::string t) override final;
};

struct Solution {
  bool isAnagram(std::string s, std::string t);
};
