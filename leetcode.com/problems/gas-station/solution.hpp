#pragma once

#include <vector>

// Problem statement: https://leetcode.com/problems/gas-station/

struct SolutionImpl {
  virtual int canCompleteCircuit(std::vector<int> &gas,
                                 std::vector<int> &cost) = 0;
};

// Leetcode score:
// Runtime
// Details 107ms Beats 85.43 % of users with C++
// Memory
// Details 128.19MB Beats 51.48 % of users with C++
struct GreedySolution : SolutionImpl {
  virtual int canCompleteCircuit(std::vector<int> &gas,
                                 std::vector<int> &cost) override final;
};

struct Solution {
  int canCompleteCircuit(std::vector<int> &gas, std::vector<int> &cost);
};
