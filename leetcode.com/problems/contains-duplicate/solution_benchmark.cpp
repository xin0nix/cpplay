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
    std::iota(nums.rbegin(), nums.rend(), (int)n / -2);
    benchmark::DoNotOptimize(nums);
    state.ResumeTiming();
    solution.containsDuplicate(nums);
  }
  state.SetComplexityN(state.range(0));
}

const size_t kThousand = 1000;
const size_t kMillion = kThousand * kThousand;
const size_t kBillion = kThousand * kMillion;

// On my machine: O(n) ~= 47 N
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, SetSolution)
    ->RangeMultiplier(10)
    ->Range(1, kMillion)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

// On my machine: O(n) ~= 0.43 NlgN
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, SortSolution)
    ->RangeMultiplier(10)
    ->Range(1, kMillion)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

// Given that multiplier for 47 N is so large, in practice 0.43 NlgN is much
// better for even very large input arrays (even millions of numbers).
// If you try to solve 47 N = 0.43 NlgN (the point after which set-based
// solution surpasses sort-based) you get a tremendous value, smth like 8*10^32.

// static void BM_Solution(benchmark::State &state) {
//   size_t n = state.range(0);
//   std::vector<int> nums(n, 0);
//   std::iota(nums.rbegin(), nums.rend(), (int)n / 2);
//   SetSolution solution;
//   for (auto _ : state) {
//     state.PauseTiming();
//     benchmark::DoNotOptimize(nums);
//     state.ResumeTiming();
//     solution.containsDuplicate(nums);
//   }
//   state.SetComplexityN(state.range(0));
// }
// BENCHMARK(BM_Solution)->RangeMultiplier(10)->Range(1, 1000000)->Complexity();

BENCHMARK_MAIN();
