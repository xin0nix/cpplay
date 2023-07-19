// solution.hpp
#pragma once

#include <vector>

// See
// The problem:
// - https://leetcode.com/problems/k-closest-points-to-origin/
// Solutions:
// This one is a poor solution based on sorted multimap
// - https://www.geeksforgeeks.org/find-k-closest-points-to-the-origin/
// - https://www.youtube.com/watch?v=rI2EBUEMfTk
//

// TODO: implement max-heap version with the limited heap size
// TODO: compare different solutions using the benchmark

using namespace std;

struct MinHeapSolution {
  vector<vector<int>> kClosest(vector<vector<int>> &points, int k);
};

struct NthElementSolution {
  vector<vector<int>> kClosest(vector<vector<int>> &points, int k);
};

class Solution {
public:
  vector<vector<int>> kClosest(vector<vector<int>> &points, int k);
};
