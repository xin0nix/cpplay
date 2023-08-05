#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  std::string str;
  for (size_t i = 0; i < n / 2; ++i)
    str.push_back('(');
  for (size_t i = n / 2; i < n; ++i)
    str.push_back(')');
  S solution;
  for (auto _ : state) {
    state.PauseTiming();
    benchmark::DoNotOptimize(str);
    state.ResumeTiming();
    solution.isValid(str);
  }
  state.SetComplexityN(state.range(0));
}

const size_t kThousand = 1000;

BENCHMARK_TEMPLATE1(BM_TemplatedSolution, StackSolution)
    ->RangeMultiplier(10)
    ->Range(1, kThousand)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
