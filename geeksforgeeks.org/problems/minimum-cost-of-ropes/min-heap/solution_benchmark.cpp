#include "solution.hpp"
#include <benchmark/benchmark.h>

using Int = long long;

static void BM_MinHeap(benchmark::State &state, Int arr[], Int n) {
  SolutionKind kind = SolutionKind::SK_MinHeap;
  for (auto _ : state) {
    Solution s;
    s.minCost(arr, n, kind);
  }
  delete[] arr; // free the memory
}

static void BM_MapPriorityQueue(benchmark::State &state, Int arr[], Int n) {
  SolutionKind kind = SolutionKind::SK_MapPQ;
  for (auto _ : state) {
    Solution s;
    s.minCost(arr, n, kind);
  }
  delete[] arr; // free the memory
}

BENCHMARK_CAPTURE(BM_MinHeap, Len1, new Int[1]{4}, 1);
BENCHMARK_CAPTURE(BM_MinHeap, Len3, new Int[3]{10, 20, 30}, 3);
BENCHMARK_CAPTURE(BM_MinHeap, Len10,
                  new Int[10]{999, 66, 127, 5, 7, 999, 9, 4789, 11, 8}, 10);
BENCHMARK_CAPTURE(BM_MinHeap, Len100,
                  new Int[100]{999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8},
                  100);

BENCHMARK_CAPTURE(BM_MapPriorityQueue, Len1, new Int[1]{4}, 1);
BENCHMARK_CAPTURE(BM_MapPriorityQueue, Len3, new Int[3]{10, 20, 30}, 3);
BENCHMARK_CAPTURE(BM_MapPriorityQueue, Len10,
                  new Int[10]{999, 66, 127, 5, 7, 999, 9, 4789, 11, 8}, 10);
BENCHMARK_CAPTURE(BM_MapPriorityQueue, Len100,
                  new Int[100]{999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8,
                               999, 66, 127, 5, 7, 999, 9, 4789, 11, 8},
                  100);

BENCHMARK_MAIN();
