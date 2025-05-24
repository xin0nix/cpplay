#include <gtest/gtest.h>

#include <queue>
#include <span>
#include <vector>

struct Solution {
  int leastInterval(std::span<const char> tasks, const int n) {
    std::priority_queue<int /*count*/> maxHeap;
    std::queue<std::pair<int /*task count*/, int /*ready time*/>> waitList;
    if (tasks.empty()) {
      return 0;
    }

    std::array<int, 26> frequencies = {0};
    for (const auto task : tasks) {
      frequencies[size_t(task) - size_t('A')]++;
    }

    for (int i = 0; i < 26; ++i) {
      if (frequencies[i] > 0) {
        maxHeap.push(frequencies[i]);
      }
    }

    int time = 0;
    while (!maxHeap.empty() || !waitList.empty()) {
      if (!waitList.empty()) {
        auto [count, readyTime] = waitList.front();
        if (time == readyTime) {
          waitList.pop();
          maxHeap.push(count);
        }
      }

      if (!maxHeap.empty()) {
        int count = maxHeap.top();
        maxHeap.pop();
        if (count - 1 > 0) {
          waitList.emplace(count - 1, time + n + 1);
        }
      }
      ++time;
    }

    return time;
  }
};

struct TaskSchedulerTestParam {
  std::vector<char> tasks;
  int n;
  int expected;
};

class TaskSchedulerTest
    : public ::testing::TestWithParam<TaskSchedulerTestParam> {};

TEST_P(TaskSchedulerTest, HandlesVariousCases) {
  const auto &param = GetParam();
  // Предполагается, что функция leastInterval уже реализована
  EXPECT_EQ(Solution{}.leastInterval(param.tasks, param.n), param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    TaskSchedulerTests, TaskSchedulerTest,
    ::testing::Values(
        // Базовый случай: tasks = ["A","A","A","B","B","B"], n = 2
        TaskSchedulerTestParam{{'A', 'A', 'A', 'B', 'B', 'B'}, 2, 8},

        // Нет необходимости в ожидании: tasks = ["A","A","A","B","B","B"], n =
        // 0
        TaskSchedulerTestParam{{'A', 'A', 'A', 'B', 'B', 'B'}, 0, 6},

        // Только одна задача: tasks = ["A"], n = 100
        TaskSchedulerTestParam{{'A'}, 100, 1},

        // Все задачи разные: tasks = ["A","B","C","D"], n = 2
        TaskSchedulerTestParam{{'A', 'B', 'C', 'D'}, 2, 4},

        // Краевой случай: пустой список задач
        TaskSchedulerTestParam{{}, 2, 0},

        // Все задачи одинаковые, большой n: tasks = ["A","A","A","A"], n = 3
        TaskSchedulerTestParam{{'A', 'A', 'A', 'A'}, 3, 13},

        // Минимальный n: tasks = ["A","B"], n = 0
        TaskSchedulerTestParam{{'A', 'B'}, 0, 2},

        // Много разных задач, n больше длины: tasks = ["A","B","C"], n = 5
        TaskSchedulerTestParam{{'A', 'B', 'C'}, 5, 3},

        // Две задачи, большой n: tasks = ["A","A","B","B"], n = 50
        TaskSchedulerTestParam{{'A', 'A', 'A', 'B', 'B'}, 50, 103}));
