#include "solution.hpp"

#include <algorithm>
#include <unordered_map>
#include <unordered_set>

std::vector<std::vector<std::string>>
MapSolution::groupAnagrams(const std::vector<std::string> &strs) {
  std::unordered_map<std::string, std::vector<std::string>> seen;
  for (const std::string &s : strs) {
    std::string k = s;
    std::sort(k.begin(), k.end());
    seen[k].push_back(s);
  }
  std::vector<std::vector<std::string>> groups;
  groups.reserve(seen.size());
  for (auto s : seen)
    groups.push_back(std::move(s.second));
  return groups;
}

std::vector<std::vector<std::string>>
Solution::groupAnagrams(std::vector<std::string> &strs) {
  return MapSolution().groupAnagrams(strs);
}
