#include "solution.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>

bool SortSolution::isAnagram(std::string s, std::string t) {
  std::sort(s.begin(), s.end());
  std::sort(t.begin(), t.end());
  return s == t;
}

bool MapSolution::isAnagram(std::string s, std::string t) {
  std::unordered_map<char, int> smap;
  std::unordered_map<char, int> tmap;
  for (char c : s)
    smap[c]++;
  for (char c : t)
    tmap[c]++;
  for (char c = 'a'; c <= 'z'; ++c)
    if (smap[c] != tmap[c])
      return false;
  return true;
}

bool ArraySolution::isAnagram(std::string s, std::string t) {
  std::fill(freq, freq + 26, 0);
  const char shift = 'a';
  for (char c : s)
    freq[c - shift]++;
  for (char c : t)
    freq[c - shift]--;
  for (int f : freq)
    if (f != 0)
      return false;
  return true;
}

bool Solution::isAnagram(std::string s, std::string t) {
  return MapSolution().isAnagram(std::move(s), std::move(t));
}
