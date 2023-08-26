#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  std::vector<int> gas(n, 0);
  std::vector<int> cost(n, 0);
  S solution;
  for (auto _ : state) {
    state.PauseTiming();
    std::iota(cost.rbegin(), cost.rend(), 1);
    std::iota(cost.begin(), cost.end(), 1);
    benchmark::DoNotOptimize(gas);
    benchmark::DoNotOptimize(cost);
    state.ResumeTiming();
    int res = solution.canCompleteCircuit(gas, cost);
    benchmark::DoNotOptimize(res);
  }
  state.SetComplexityN(state.range(0));
}

const size_t kThousand = 1000;
const size_t kMillion = kThousand * kThousand;
const size_t kBillion = kThousand * kMillion;

// 0.91 N with 4% RMS on my machine
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, GreedySolution)
    ->RangeMultiplier(10)
    ->Range(1, kMillion)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oN);

BENCHMARK_MAIN();
