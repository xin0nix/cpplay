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
    // trivial case is just fine
    std::iota(nums.begin(), nums.end(), 0);
    benchmark::DoNotOptimize(nums);
    state.ResumeTiming();
    solution.maxSubArray(nums);
  }
  state.SetComplexityN(state.range(0));
}

const size_t kThousand = 1000;
const size_t kMillion = kThousand * kThousand;
const size_t kBillion = kThousand * kMillion;

// 0.39 N^2 on my laptop
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BruteForceSolution)
    ->RangeMultiplier(10)
    ->Range(1, 100 * kThousand)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oNSquared);

// 0.86 NlgN on my laptop
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, DivideConquerSolution)
    ->RangeMultiplier(10)
    ->Range(1, 100 * kThousand)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oNLogN);

// 1.02 N on my laptop
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, SlidingWindowSolution)
    ->RangeMultiplier(10)
    ->Range(1, 100 * kThousand)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oN);

BENCHMARK_MAIN();
