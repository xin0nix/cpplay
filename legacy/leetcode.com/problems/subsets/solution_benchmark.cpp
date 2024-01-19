#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  std::vector<int> nums(n, 0);
  S solution;
  for (auto _ : state) {
    state.PauseTiming();
    std::iota(nums.begin(), nums.end(), 1);
    benchmark::DoNotOptimize(nums);
    state.ResumeTiming();
    auto res = solution.subsets(nums);
    benchmark::DoNotOptimize(res);
  }
  state.SetComplexityN(state.range(0));
}

// On my laptop it's 75.78 N^3 with 43% confidence
// Expected copmlexity is about N*2^N but I believe gtest just doesn't take it
// into account when trying to figure out the best regression model over N
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BasicSolution)
    ->DenseRange(1, 10, 1)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
