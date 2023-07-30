#include "solution.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>

// see:
// https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
std::string getRandomString(const int len) {
  static const char alpha[] = "abcdefghijklmnopqrstuvwxyz";
  std::string res;
  res.reserve(len);
  for (int i = 0; i < len; ++i)
    res += alpha[rand() % (sizeof(alpha) - 1)];
  return res;
}

template <typename S>
static void BM_TemplatedSolution(benchmark::State &state) {
  size_t n = state.range(0);
  const std::vector<std::string> words = [n]() {
    std::vector<std::string> r;
    r.reserve(n);
    for (size_t i = 0; i < n; ++i)
      r.push_back(getRandomString(5));
    return r;
  }();
  S solution;
  for (auto _ : state) {
    state.PauseTiming();
    benchmark::DoNotOptimize(words);
    state.ResumeTiming();
    auto res = solution.groupAnagrams(words);
    benchmark::DoNotOptimize(res);
  }
  state.SetComplexityN(state.range(0));
}

const size_t kThousand = 1000;
const size_t kMillion = kThousand * kThousand;
const size_t kBillion = kThousand * kMillion;

BENCHMARK_TEMPLATE1(BM_TemplatedSolution, MapSolution)
    ->RangeMultiplier(10)
    ->Range(1, kMillion)
    ->Unit(benchmark::kMicrosecond)
    ->Complexity();

BENCHMARK_MAIN();
