#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>

static void BM_Solution(benchmark::State &state) {
  for (auto _ : state) {
    Solution solution;
    std::vector<int> nums;
    nums.reserve(999);
    for (int i = 0; i < 999; ++i)
      nums.push_back(i);
    benchmark::DoNotOptimize(solution.lastStoneWeight(nums));
  }
}

BENCHMARK(BM_Solution);

BENCHMARK_MAIN();
