#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  S solution;
  std::string s;
  std::string t;
  for (auto _ : state) {
    state.PauseTiming();
    benchmark::DoNotOptimize(s);
    benchmark::DoNotOptimize(t);
    s.clear();
    t.clear();
    while (s.size() < n) {
      // not an anagram
      s += "azyxwvutsrqponmlkjihgfedcba";
      t += "bzyxwvutsrqponmlkjihgfedcba";
    }
    state.ResumeTiming();
    solution.isAnagram(s, t);
  }
  state.SetComplexityN(state.range(0));
}

const size_t kThousand = 1000;
const size_t kMillion = kThousand * kThousand;
const size_t kBillion = kThousand * kMillion;

// On my machine: O(n) ~= 1.68 NlgN
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, SortSolution)
    ->RangeMultiplier(10)
    ->Range(1, 10 * kMillion)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oNLogN);

// On my machine: O(n) ~= 7.79 N
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, MapSolution)
    ->RangeMultiplier(10)
    ->Range(1, 10 * kMillion)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oN);

// On my machine: O(n) ~= 1.23 N
BENCHMARK_TEMPLATE1(BM_TemplatedSolution, ArraySolution)
    ->RangeMultiplier(10)
    ->Range(1, 10 * kMillion)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity(benchmark::oN);

BENCHMARK_MAIN();
