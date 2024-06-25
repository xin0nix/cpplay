#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  S solution;
  // clang-format off
  vector<vector<char>> board{
     {'8','3','.','.','7','.','.','.','.'}
    ,{'6','.','.','1','9','5','.','.','.'}
    ,{'.','9','8','.','.','.','.','6','.'}
    ,{'8','.','.','.','6','.','.','.','3'}
    ,{'4','.','.','8','.','3','.','.','1'}
    ,{'7','.','.','.','2','.','.','.','6'}
    ,{'.','6','.','.','.','.','2','8','.'}
    ,{'.','.','.','4','1','9','.','.','5'}
    ,{'.','.','.','.','8','.','.','7','9'}
  };
  // clang-format on
  for (auto _ : state) {
    state.PauseTiming();
    benchmark::DoNotOptimize(board);
    state.ResumeTiming();
    bool res = solution.isValidSudoku(board);
    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BasicSolution);

BENCHMARK_MAIN();
