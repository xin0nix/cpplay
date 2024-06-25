#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  S solution;
  std::vector<int> cost(n, 0);
  for (auto _ : state) {
    state.PauseTiming();
    std::iota(cost.begin(), cost.end(), 1);
    state.ResumeTiming();
    int res = solution.minCostClimbingStairs(cost);
    benchmark::DoNotOptimize(res);
  }
  state.SetComplexityN(state.range(0));
}

const size_t kThousand = 1000;
const size_t kMillion = kThousand * kThousand;
const size_t kBillion = kThousand * kMillion;

// 1N, rms 1%
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BottomUpSolution)
    ->RangeMultiplier(10)
    ->Range(1, 100 * kThousand)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

// 4 NlgN, rms 2% (lg probably comes from rehashing)
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BFSolution)
    ->RangeMultiplier(10)
    ->Range(1, 100 * kThousand)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
