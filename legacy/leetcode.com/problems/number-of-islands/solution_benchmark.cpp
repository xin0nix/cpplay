#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  std::vector<std::vector<char>> grid;
  for (size_t i = 0; i < n; ++i) {
    grid.push_back(std::vector<char>(n, '0'));
  }
  S solution;
  for (auto _ : state) {
    state.PauseTiming();
    for (size_t i = 0; i < n; ++i)
      for (size_t j = 0; j < n; ++j)
        if (i % 1 || i % 2 || j % 4)
          grid[i][j] = '1';
        else
          grid[i][j] = '0';
    benchmark::DoNotOptimize(grid);
    state.ResumeTiming();
    solution.numIslands(grid);
  }
  state.SetComplexityN(n * n);
}

BENCHMARK_TEMPLATE1(BM_TemplatedSolution, SetNDequeSolution)
    ->RangeMultiplier(2)
    ->Range(2, 256)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_TEMPLATE1(BM_TemplatedSolution, RewriterSolution)
    ->RangeMultiplier(2)
    ->Range(2, 256)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
