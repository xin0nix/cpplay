#include "solution.hpp"
#include <benchmark/benchmark.h>

static void BM_Solution(benchmark::State& state) {
  Solution solution;
  for (auto _ : state) {
    benchmark::DoNotOptimize(solution.solve());
  }
}

BENCHMARK(BM_Solution);

BENCHMARK_MAIN();
