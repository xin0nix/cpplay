#include "solution.hpp"
#include <benchmark/benchmark.h>

using Int = long long;

template <size_t size, SolutionKind kind>
static void BM_Solution(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Int numbers[size];
    for (size_t i = 0; i < size; ++i)
      numbers[i] = i % 37;
    state.ResumeTiming();
    Solution s;
    s.minCost(numbers, size, kind);
  }
}

constexpr size_t Thousand = 1000;
constexpr size_t Million = Thousand * Thousand;
BENCHMARK(BM_Solution<10, SolutionKind::SK_MapPQ>);
BENCHMARK(BM_Solution<10, SolutionKind::SK_MinHeap>);
BENCHMARK(BM_Solution<10, SolutionKind::SK_STDMinHeap>);
BENCHMARK(BM_Solution<Thousand, SolutionKind::SK_MapPQ>);
BENCHMARK(BM_Solution<Thousand, SolutionKind::SK_MinHeap>);
BENCHMARK(BM_Solution<Thousand, SolutionKind::SK_STDMinHeap>);
BENCHMARK(BM_Solution<10 * Thousand, SolutionKind::SK_MapPQ>);
BENCHMARK(BM_Solution<10 * Thousand, SolutionKind::SK_MinHeap>);
BENCHMARK(BM_Solution<10 * Thousand, SolutionKind::SK_STDMinHeap>);

BENCHMARK_MAIN();
