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
    solution.productExceptSelf(nums);
  }
  state.SetComplexityN(state.range(0));
}

const size_t kThousand = 1000;
const size_t kMillion = kThousand * kThousand;
const size_t kBillion = kThousand * kMillion;

BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BasicSolution)
    ->RangeMultiplier(10)
    ->Range(1, kMillion)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
