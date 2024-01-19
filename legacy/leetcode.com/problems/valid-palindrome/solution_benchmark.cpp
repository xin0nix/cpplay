#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  std::string phrase;
  const std::string chunk = "abcdefghkj";
  while (phrase.size() < n / 2)
    phrase += chunk;
  std::string phraseReverse = phrase;
  std::reverse(phraseReverse.begin(), phraseReverse.end());
  phrase += phraseReverse;
  S solution;
  for (auto _ : state) {
    state.PauseTiming();
    std::string phraseCopy = phrase;
    benchmark::DoNotOptimize(phraseCopy);
    state.ResumeTiming();
    solution.isPalindrome(phraseCopy);
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK_TEMPLATE1(BM_TemplatedSolution, BruteForceSolution)
    ->RangeMultiplier(2)
    ->Range(20, 1024)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_TEMPLATE1(BM_TemplatedSolution, TwoPointerSolution)
    ->RangeMultiplier(2)
    ->Range(20, 1024)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
