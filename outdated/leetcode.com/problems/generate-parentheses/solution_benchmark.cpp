#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  int n = state.range(0);
  S solution;
  for (auto _ : state) {
    auto parentheses = solution.generateParenthesis(n);
    benchmark::DoNotOptimize(parentheses);
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BactrackingSolution)
    ->DenseRange(1, 10, 1)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
