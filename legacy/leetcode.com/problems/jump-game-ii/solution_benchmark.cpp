#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  const std::vector<int> pattern{
      2, 1, 6, 9, 8, 4, 5, 1, 8, 9, 1, 2, 9, 6, 8, 8, 1, 6, 3, 1,
      2, 2, 1, 2, 6, 5, 3, 1, 2, 2, 6, 4, 2, 4, 3, 1, 2, 1, 3, 8,
      2, 4, 1, 1, 2, 1, 1, 4, 6, 5, 8, 1, 7, 9, 3, 4, 6, 6, 5, 8,
      9, 3, 4, 3, 7, 1, 4, 9, 1, 9, 8, 4, 3, 1, 7, 7, 1, 9, 1, 9,
      4, 9, 1, 1, 9, 5, 7, 7, 1, 5, 8, 2, 8, 2, 6, 8, 2, 2, 7, 5};
  std::vector<int> nums(n, 0);
  S solution;
  for (auto _ : state) {
    state.PauseTiming();
    for (size_t i = 0UL; i < n; ++i)
      nums[i] = pattern[i % pattern.size()];
    benchmark::DoNotOptimize(nums);
    state.ResumeTiming();
    bool res = solution.jump(nums);
    benchmark::DoNotOptimize(res);
  }
  state.SetComplexityN(state.range(0));
}

const size_t kThousand = 1000;
const size_t kMillion = kThousand * kThousand;
const size_t kBillion = kThousand * kMillion;

BENCHMARK_TEMPLATE1(BM_TemplatedSolution, GreedySolution)
    ->RangeMultiplier(100)
    ->Range(1, 100 * kThousand)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
