#include "solution.hpp"
#include <benchmark/benchmark.h>

using Int = long long;

static void BM_SolutionCustomSort(benchmark::State &state, Int arr[], Int n) {
  SolutionKind kind = SolutionKind::SK_MinHeap;
  for (auto _ : state) {
    Solution s;
    s.minCost(arr, n);
  }
  delete[] arr; // free the memory
}

BENCHMARK_CAPTURE(BM_SolutionCustomSort, Len1, new Int[1]{4}, 1);
BENCHMARK_CAPTURE(BM_SolutionCustomSort, Len3, new Int[3]{10, 20, 30}, 3);
BENCHMARK_CAPTURE(BM_SolutionCustomSort, Len10,
                  new Int[10]{999, 66, 127, 5, 7, 999, 9, 4789, 11, 8}, 10);

BENCHMARK_MAIN();
