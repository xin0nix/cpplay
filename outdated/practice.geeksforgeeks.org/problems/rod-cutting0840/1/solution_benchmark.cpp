#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  int *price = new int[n]();
  S solution;
  for (auto _ : state) {
    state.PauseTiming();
    std::iota(price, price + n, 1);
    benchmark::DoNotOptimize(price);
    state.ResumeTiming();
    int profit = solution.cutRod(price, n);
    benchmark::DoNotOptimize(profit);
  }
  delete[] price;
  state.SetComplexityN(state.range(0));
}

// 1.30 N^2 on my laptop
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BasicSolution)
    ->RangeMultiplier(2)
    ->Range(1, 1024)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

// 0.11 N^2 on my laptop, same complexity, lower constant (fits the theory)
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BottomUpSolution)
    ->RangeMultiplier(2)
    ->Range(1, 1024)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
