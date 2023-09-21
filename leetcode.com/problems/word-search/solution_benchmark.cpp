#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  vector<vector<char>> board(n, vector<char>(n, 'a'));
  string word;
  for (size_t i = 0; i < n; ++i)
    word.push_back('a' + rand() % 26);
  S solution;
  for (auto _ : state) {
    state.PauseTiming();
    for (size_t i = 0; i < n; ++i)
      for (size_t j = 0; j < n; ++j)
        board[i][j] = 'a' + rand() % 26;
    benchmark::DoNotOptimize(board);
    state.ResumeTiming();
    bool res = solution.exist(board, word);
    benchmark::DoNotOptimize(res);
  }
  state.SetComplexityN(n * n);
}

const size_t kThousand = 1000;
const size_t kMillion = kThousand * kThousand;
const size_t kBillion = kThousand * kMillion;

// 1.77 N (suspiciously low)
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BasicSolution)
    ->RangeMultiplier(10)
    ->Range(1, kThousand)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
