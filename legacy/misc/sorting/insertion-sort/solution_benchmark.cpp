#include "solution.hpp"
#include <benchmark/benchmark.h>
#include <execution>
#include <vector>

struct MySort {
  void operator()(std::vector<int> &numbers) {
    Solution solution;
    solution.sort(numbers);
  }
};

struct StdSort {
  void operator()(std::vector<int> &numbers) {
    std::sort(numbers.begin(), numbers.end());
  }
};

template <typename T, size_t Size>
static void BM_Solution(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<int> numbers;
    for (size_t i = 0; i < Size; ++i)
      numbers.push_back(i % 37);
    state.ResumeTiming();
    T f;
    f(numbers);
  }
}

constexpr size_t Thousand = 1000;
constexpr size_t Million = Thousand * Thousand;
BENCHMARK(BM_Solution<MySort, 10>);
BENCHMARK(BM_Solution<StdSort, 10>);
BENCHMARK(BM_Solution<MySort, Thousand>);
BENCHMARK(BM_Solution<StdSort, Thousand>);
BENCHMARK(BM_Solution<MySort, 10 * Thousand>);
BENCHMARK(BM_Solution<StdSort, 10 * Thousand>);

BENCHMARK_MAIN();
