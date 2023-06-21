#include "solution.hpp"

//{ Driver Code Starts.
#include <iostream>
using namespace std;

int main() {
  long long arr[] = {1, 2, 3};
  long long n = sizeof(arr) / sizeof(long long);
  std::cout << "n = " << n << "\n";
  Solution s;
  long long actual = s.minCost(arr, n, SolutionKind::SK_STDMinHeap);
  std::cout << "Result " << actual << std::endl;
}
