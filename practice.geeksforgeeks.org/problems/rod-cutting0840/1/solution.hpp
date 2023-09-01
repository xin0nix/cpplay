#pragma once

#include <vector>

// Problem statement:
// https://practice.geeksforgeeks.org/problems/rod-cutting0840/1

struct SolutionImpl {
  virtual int cutRod(int price[], const int n) = 0;
};

// GFG: 0.04 ms
// Accuracy: 100%, Attempts: 1
struct BasicSolution : SolutionImpl {
  virtual int cutRod(int price[], const int n) override final;
};

// GFG: 0.03 ms
// Accuracy: 100%, Attempts: 1 (3 correct submissions in a row)
struct BottomUpSolution : SolutionImpl {
  virtual int cutRod(int price[], const int n) override final;
};

// TODO: implement a faster, bottom-up solution!

struct Solution {
  int cutRod(int price[], int n);
};
