#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  S solution;
  for (auto _ : state) {
    state.PauseTiming();
    state.ResumeTiming();
    int res = solution.climbStairs(n);
    benchmark::DoNotOptimize(res);
  }
  state.SetComplexityN(state.range(0));
}

const size_t kThousand = 1000;
const size_t kMillion = kThousand * kThousand;
const size_t kBillion = kThousand * kMillion;

// 12N
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BasicSolution)
    ->RangeMultiplier(10)
    ->Range(1, 100 * kThousand)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oN);

// 0.25N
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BottomUpSolution)
    ->RangeMultiplier(10)
    ->Range(1, 100 * kThousand)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oN);

BENCHMARK_MAIN();
